
#include "ttfsessionSqDet.h"

namespace TTF
{


TTfSessionSqDet::TTfSessionSqDet()
{

}

TTfSessionSqDet::~TTfSessionSqDet(void)
{

}

bool TTfSessionSqDet::InitModel(const std::string &file_name, const double &gpu_fraction, const int& device_number)
{
    if(!SetConfigParams(ConfigPath))
        return false;

    if(!TTfSession::InitModel(file_name,gpu_fraction,device_number))
        return false;

    return true;
}


bool TTfSessionSqDet::SetConfigParams(const std::string& config_path)
{
    try
    {
        boost::property_tree::ptree root;

        //Парсинг файла конфигурации
        boost::property_tree::read_json(config_path, root);

        // Считывание значений
        AnchorHeight    = root.get<int>("ANCHORS_HEIGHT", 0);
        AnchorWidth     = root.get<int>("ANCHORS_WIDTH", 0);
        AnchorPerGrid   = root.get<int>("ANCHOR_PER_GRID", 0);
        MaxDetections   = root.get<int>("TOP_N_DETECTION", 0);
        ImgHeight       = root.get<int>("IMAGE_HEIGHT", 0);
        ImgWidth        = root.get<int>("IMAGE_WIDTH", 0);
        NumberOfClasses = root.get<int>("CLASSES", 0);

        ExpThresh       = root.get<float>("EXP_THRESH", 0);
        NMthreshold     = root.get<float>("NMS_THRESH", 0);
        ProbThreshold   = root.get<float>("PROB_THRESH", 0);


        std::vector<float> anchor_seeds;
        //auto anchor_seed=root.get<std::vector<float>>("ANCHOR_SEED",{});

        for (boost::property_tree::ptree::value_type &row : root.get_child("ANCHOR_SEED"))
        {
            for (boost::property_tree::ptree::value_type &cell : row.second)
            {
                anchor_seeds.push_back(cell.second.get_value<float>());
            }

        }
        AnchorSeedValues=anchor_seeds;

    }
    catch (std::exception const& e)
    {
        ErCode=TfErrorCode::EXCEPTION;
        ExceptionString=e.what();
        return false;
    }

    if(AnchorSeedValues.size()!=AnchorPerGrid*2)
    {
        ErCode=TfErrorCode::BAD_CONFIG;
        return false;
    }

    if(!CreatePostProcGraph())
    {
        return false;
    }
    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSessionSqDet::CreatePostProcGraph()
{
    ///Выходной вектор для преобразовани входного тензора
    std::vector<tensorflow::Tensor> OutputForProc;

    ///Граф для преобрзования входного тензора
    tensorflow::GraphDef GraphForProc;

    //Создание графа вычислений для преобразования входной тензор в нужный вид
    tensorflow::Scope root = tensorflow::Scope::NewRootScope();


    //Кусок графа для рассчёта AnchorBox и расчёт AnchorBox
    tensorflow::Tensor anchor_seed(tensorflow::DataType::DT_FLOAT, tensorflow::TensorShape({AnchorPerGrid,2}));
    auto tmp_data = anchor_seed.flat<float>();


    for(int i=0;i<9*2;i++)
    {
        tmp_data(i)=AnchorSeedValues[i];
    }

    //anchor_shapes
    auto constant=tensorflow::ops::Const(root,anchor_seed);
    auto tiled=tensorflow::ops::Tile(root,constant,{AnchorHeight*AnchorWidth,1});
    auto reshaped_a=tensorflow::ops::Reshape(root,tiled,{AnchorHeight,AnchorWidth,AnchorPerGrid,2});

    //enter_x
    auto range=tensorflow::ops::Range(root,1,AnchorWidth+1,1);
    auto multiplicate=tensorflow::ops::Multiply(root,
                                                tensorflow::ops::Cast(root,range,tensorflow::DataType::DT_FLOAT),{float(ImgWidth)/float((AnchorWidth+1))});
    auto tiled_1=tensorflow::ops::Tile(root,multiplicate,{AnchorHeight*AnchorPerGrid});
    auto reshaped_a1 = tensorflow::ops::Reshape(root,tiled_1,{AnchorPerGrid,AnchorHeight,AnchorWidth});
    auto transposed = tensorflow::ops::Transpose(root,reshaped_a1,{1,2,0});
    auto reshaped_a12 = tensorflow::ops::Reshape(root,transposed,{AnchorHeight,AnchorWidth,AnchorPerGrid,1});

    //center_y
    auto range_2=tensorflow::ops::Range(root,1,AnchorHeight+1,1);
    auto multiplicate_2=tensorflow::ops::Multiply(root,
                                                  tensorflow::ops::Cast(root,range_2,tensorflow::DataType::DT_FLOAT),{float(ImgHeight)/float((AnchorHeight+1))});
    auto tiled_2=tensorflow::ops::Tile(root,multiplicate_2,{AnchorWidth*AnchorPerGrid});
    auto reshaped_a2 = tensorflow::ops::Reshape(root,tiled_2,{AnchorPerGrid,AnchorWidth,AnchorHeight});
    auto transposed_2 = tensorflow::ops::Transpose(root,reshaped_a2,{2,1,0});
    auto reshaped_22 = tensorflow::ops::Reshape(root,transposed_2,{AnchorHeight,AnchorWidth,AnchorPerGrid,1});

    //anchors
    auto concatenated=tensorflow::ops::Concat(root,std::initializer_list<tensorflow::Input>({reshaped_a12,reshaped_22,reshaped_a}),3);
    auto reshaped_a3 = tensorflow::ops::Reshape(root.WithOpName("AnchorBox"),concatenated,{-1,4});

   Status = root.ToGraphDef(&GraphForProc);
   if(!Status.ok())
   {
       ErCode = TfErrorCode::BAD_STATUS;
       return false;
   }

   //Определение параметров сессии
   tensorflow::SessionOptions opts;
   opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.6);
   opts.config.mutable_gpu_options()->set_allow_growth(true);

    //Инициализация сессии
    Status = tensorflow::NewSession(opts, &SessionForPostProc);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //Добавление графа в сессию
    Status = SessionForPostProc->Create(GraphForProc);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    Status = SessionForPostProc->Run({},{"AnchorBox",},{},&OutputForProc);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    AnchorBoxes = OutputForProc[0];
    Anchors=AnchorBoxes.dim_size(0);

    OutputForProc.clear();
    GraphForProc.Clear();
    Status = SessionForPostProc->Close();
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }



    //Кусок графа постобработки
    int n_outputs=NumberOfClasses+1+4;
    auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_FLOAT);

    auto strided = tensorflow::ops::StridedSlice(root, a,{0,0,0},{1,16848,n_outputs},{1,1,1});

    auto reshaped = tensorflow::ops::Reshape(root,strided,{1,AnchorHeight,AnchorWidth,16848*8/(AnchorHeight*AnchorWidth)});

    //////////
    int num_class_probs = AnchorPerGrid * NumberOfClasses;
    auto strided_1 = tensorflow::ops::StridedSlice(root, reshaped,{0,0,0,0},{1,AnchorHeight,AnchorWidth,num_class_probs},{1,1,1,1});
    auto reshaped_1 = tensorflow::ops::Reshape(root,strided_1,{Anchors,NumberOfClasses});
    auto softmax = tensorflow::ops::Softmax(root,reshaped_1);
    auto reshaped_12 =tensorflow::ops::Reshape(root.WithOpName("pred_class_probs"),softmax,{1,Anchors,NumberOfClasses});

    //////////
    int num_confidence_scores = AnchorPerGrid + num_class_probs;
    auto strided_2 = tensorflow::ops::StridedSlice(root, reshaped,{0,0,0,num_class_probs},{1,AnchorHeight,AnchorWidth,num_confidence_scores},{1,1,1,1});
    auto reshaped_2 = tensorflow::ops::Reshape(root,strided_2,{1,Anchors});
    auto sigmoid=tensorflow::ops::Sigmoid(root.WithOpName("pred_conf"),reshaped_2);

    //////////
    auto strided_3 = tensorflow::ops::StridedSlice(root, reshaped,{0,0,0,num_confidence_scores},{1,AnchorHeight,AnchorWidth,16848*8/(AnchorHeight*AnchorWidth)},{1,1,1,1});
    auto pred_box_delta = tensorflow::ops::Reshape(root.WithOpName("pred_box_delta"),tensorflow::ops::Reshape(root,strided_3,{1,Anchors,4}),{Anchors,4});

    //Кусок графа для постобработки Boxes
    auto anchors=tensorflow::ops::Const(root,AnchorBoxes);

    auto anchor_x = tensorflow::ops::StridedSlice(root, anchors,{0,0},{16848,1},{1,1});
    auto anchor_y = tensorflow::ops::StridedSlice(root, anchors,{0,1},{16848,2},{1,1});
    auto anchor_w = tensorflow::ops::StridedSlice(root, anchors,{0,2},{16848,3},{1,1});
    auto anchor_h = tensorflow::ops::StridedSlice(root, anchors,{0,3},{16848,4},{1,1});

    auto delta_x = tensorflow::ops::StridedSlice(root, pred_box_delta,{0,0},{16848,1},{1,1});
    auto delta_y = tensorflow::ops::StridedSlice(root, pred_box_delta,{0,1},{16848,2},{1,1});
    auto delta_w = tensorflow::ops::StridedSlice(root, pred_box_delta,{0,2},{16848,3},{1,1});
    auto delta_h = tensorflow::ops::StridedSlice(root, pred_box_delta,{0,3},{16848,4},{1,1});


    auto box_center_x=tensorflow::ops::Add(root,anchor_x,tensorflow::ops::Multiply(root,delta_x,anchor_w));
    auto box_center_y=tensorflow::ops::Add(root,anchor_y,tensorflow::ops::Multiply(root,delta_y,anchor_h));


    //safe exp
    auto thresh=tensorflow::ops::Fill(root,{16848,1},ExpThresh);

    auto slope=tensorflow::ops::Exp(root,thresh);


    // safe_exp_w
    auto lin_bool_w   = tensorflow::ops::Greater(root,
                                                delta_w,
                                                ExpThresh);
    auto lin_region_w = tensorflow::ops::Cast(root,lin_bool_w,tensorflow::DataType::DT_FLOAT);

    auto lin_out_w = tensorflow::ops::Multiply(root,slope,
                                            tensorflow::ops::Add(root,tensorflow::ops::Fill(root,{16848,1},1.0f),
                                                                tensorflow::ops::Subtract(root,delta_w,thresh)));

    auto exp_out_w=tensorflow::ops::Exp(root,tensorflow::ops::Where3(root,
                                                                   lin_bool_w,
                                                                   tensorflow::ops::ZerosLike(root,delta_w),
                                                                   delta_w));

    auto safe_exp_w=tensorflow::ops::Add(root,
                                  tensorflow::ops::Multiply(root,lin_region_w,lin_out_w),
                                  tensorflow::ops::Multiply(root,
                                                            tensorflow::ops::Subtract(root,
                                                                                      tensorflow::ops::Fill(root,{16848,1},1.0f),lin_region_w),exp_out_w));
    //safe_exp_h
    auto lin_bool_h   = tensorflow::ops::Greater(root,
                                               delta_w,
                                               ExpThresh);
    auto lin_region_h = tensorflow::ops::Cast(root,lin_bool_h,tensorflow::DataType::DT_FLOAT);

    auto lin_out_h = tensorflow::ops::Multiply(root,slope,
                                            tensorflow::ops::Add(root,tensorflow::ops::Fill(root,{16848,1},1.0f),
                                                                tensorflow::ops::Subtract(root,delta_h,thresh)));

    auto exp_out_h=tensorflow::ops::Exp(root,tensorflow::ops::Where3(root,
                                                                    lin_bool_h,
                                                                    tensorflow::ops::ZerosLike(root,delta_h),
                                                                    delta_h));

    auto safe_exp_h=tensorflow::ops::Add(root,
                                  tensorflow::ops::Multiply(root,lin_region_h,lin_out_h),
                                  tensorflow::ops::Multiply(root,
                                                            tensorflow::ops::Subtract(root,
                                                                                      tensorflow::ops::Fill(root,{16848,1},1.0f),lin_region_h),exp_out_h));


    //
    auto box_width  =   tensorflow::ops::Multiply(root,anchor_w,safe_exp_w);
    auto box_height =   tensorflow::ops::Multiply(root,anchor_h,safe_exp_h);

    auto x1 = tensorflow::ops::Subtract(root,box_center_x,tensorflow::ops::Div(root,box_width,tensorflow::ops::Fill(root,{16848,1},2.0f)));
    auto y1 = tensorflow::ops::Subtract(root,box_center_y,tensorflow::ops::Div(root,box_height,tensorflow::ops::Fill(root,{16848,1},2.0f)));
    auto x2 = tensorflow::ops::Add(root,box_center_x,tensorflow::ops::Div(root,box_width,tensorflow::ops::Fill(root,{16848,1},2.0f)));;
    auto y2 = tensorflow::ops::Add(root,box_center_y,tensorflow::ops::Div(root,box_height,tensorflow::ops::Fill(root,{16848,1},2.0f)));;

    auto xmins = tensorflow::ops::Minimum(root,
                                         tensorflow::ops::Maximum(root,tensorflow::ops::Fill(root,{16848,1},0.0f),x1),
                                         tensorflow::ops::Fill(root,{16848,1},float(ImgWidth-1.0)));

    auto ymins = tensorflow::ops::Minimum(root,
                                         tensorflow::ops::Maximum(root,tensorflow::ops::Fill(root,{16848,1},0.0f),y1),
                                         tensorflow::ops::Fill(root,{16848,1},float(ImgHeight-1.0)));

    auto xmaxs = tensorflow::ops::Maximum(root,
                                         tensorflow::ops::Minimum(root,tensorflow::ops::Fill(root,{16848,1},float(ImgWidth-1.0)),x2),
                                         tensorflow::ops::Fill(root,{16848,1},0.0f));

    auto ymaxs = tensorflow::ops::Maximum(root,
                                         tensorflow::ops::Minimum(root,tensorflow::ops::Fill(root,{16848,1},float(ImgHeight-1.0)),y2),
                                         tensorflow::ops::Fill(root,{16848,1},0.0f));

    // формат [y1 x1 y2 x2]
    auto det_boxes=tensorflow::ops::Concat(root.WithOpName("det_boxes"),std::initializer_list<tensorflow::Input>({ymins,xmins,ymaxs,xmaxs}),1);
    //Конец преобработки бокса


    //////////
    auto reshaped_4 = tensorflow::ops::Reshape(root,sigmoid,{1,Anchors,1});
    auto probs = tensorflow::ops::Mul(root.WithOpName("probs"),reshaped_12,reshaped_4);

    /////////
    auto det_probs=tensorflow::ops::Reshape(root.WithOpName("det_probs"),tensorflow::ops::Max(root,probs,2),{Anchors});
    auto det_class=tensorflow::ops::Reshape(root.WithOpName("det_class"),tensorflow::ops::ArgMax(root,probs,2),{Anchors});



    //подавление не максимумов и окончательный результа
    auto indeces = tensorflow::ops::NonMaxSuppressionV3(root.WithOpName("indeces"), det_boxes, det_probs,
                                                        MaxDetections,NMthreshold,ProbThreshold);


    auto new_scores     = tensorflow::ops::GatherV2(root.WithOpName("Scores"),det_probs,indeces,int(0));
    auto new_classes    = tensorflow::ops::GatherV2(root.WithOpName("Classes"),det_class,indeces,int(0));
    auto new_boxes      = tensorflow::ops::GatherV2(root.WithOpName("Boxes"),det_boxes,indeces,int(0));

    Status = root.ToGraphDef(&GraphForProc);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

     //Инициализация сессии
     Status = tensorflow::NewSession(tensorflow::SessionOptions(), &SessionForPostProc);
     if(!Status.ok())
     {
         ErCode = TfErrorCode::BAD_STATUS;
         return false;
     }
     //Добавление графа в сессию
     Status = SessionForPostProc->Create(GraphForProc);
     if(!Status.ok())
     {
         ErCode = TfErrorCode::BAD_STATUS;
         return false;
     }
     ErCode = TfErrorCode::OK;
     return true;

}

bool TTfSessionSqDet::SetConfigPath(const std::string& path)
{
    ConfigPath=path;
    return true;
}

bool TTfSessionSqDet::Run(void)
{
    if(!TTfSession::Run())
        return false;

    if(!FilterPredicton())
        return false;


    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSessionSqDet::FilterPredicton()
{
    auto input = Output[0];

    Status = SessionForPostProc->Run({{"input",input},},{"Boxes","Classes","Scores"},{},&Output);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    ErCode = TfErrorCode::OK;
    return true;

}

bool TTfSessionSqDet::SetInputDataCvMeth(cv::Mat& image)
{
    cv::Scalar cv_mean, cv_stddev;
    cv::meanStdDev(image, cv_mean, cv_stddev);

    float mean = (cv_mean[0]+cv_mean[1]+cv_mean[2])/3;
    float stddev = (cv_stddev[0]+cv_stddev[1]+cv_stddev[2])/3;

    if(!SetImgParams({mean,mean,mean},stddev,ImgBgr))
        return false;

    if(!TTfSession::SetInputDataCvMeth(image))
        return false;
    ErCode = TfErrorCode::OK;
    return true;
}


}
