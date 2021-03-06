/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <graph/debug/ge_attr_define.h>

namespace ge {
// Public attribute
const std::string ATTR_NAME_NAME = "name";

const std::string ATTR_NAME_TYPE = "type";

const std::string ATTR_NAME_WEIGHT_NAME = "weight_name";

const std::string ATTR_NAME_IS_QUANTIZE_FACTOR = "quantize_factor";

const std::string ATTR_NAME_ALPHA = "alpha";

const std::string ATTR_NAME_BETA = "beta";

const std::string ATTR_NAME_PADMODE = "pad_mode";

const std::string ATTR_NAME_PADMODES = "padding";

const std::string ATTR_NAME_MODE = "mode";

const std::string ATTR_NAME_FILTER = "filter";

const std::string ATTR_NAME_BIAS = "bias";

const std::string ATTR_NAME_BIAS_TERM = "bias_term";

const std::string ATTR_NAME_PAD = "pad";

const std::string ATTR_NAME_PADS = "pad";

const std::string ATTR_NAME_PAD_SIZE = "pad size";

const std::string ATTR_NAME_PAD_MODE = "pad mode";

const std::string ATTR_NAME_SCALE = "scale";

const std::string ATTR_NAME_WINDOWS = "windows";

const std::string ATTR_NAME_GLOBAL_POOLING = "global_pooling";

const std::string ATTR_NAME_CEIL_MODE = "ceil_mode";

const std::string ATTR_NAME_RELUMODE = "relu_mode";

const std::string ATTR_NAME_STRIDE_SIZE = "stride size";

const std::string ATTR_NAME_RELU_FLAG = "relu_flag";

const std::string ATTR_NAME_ALGO = "algo";

const std::string ATTR_NAME_FORMAT = "format";

const std::string ATTR_NAME_FILTER_FORMAT = "filter_format";

const std::string ATTR_NAME_LRN_K = "lrn_k";

const std::string ATTR_NAME_LRN_NORM_REGION = "lrn_normregion";

const std::string ATTR_NAME_LRN_LOCAL_SIZE = "lrn_localsize";

const std::string ATTR_NAME_LRN_ALPHA = "lrn_alpha";

const std::string ATTR_NAME_LRN_BETA = "lrn_beta";

const std::string ATTR_NAME_AXIS = "axis";
const std::string ATTR_NAME_BROADCAST = "broadcast";

const std::string ATTR_NAME_OUTPUT_NUM = "output_num";
const std::string ATTR_NAME_TIDX = "t_idx";

const std::string ATTR_NAME_TPADDINGS = "t_paddings";
const std::string ATTR_IMG_H = "img_h";
const std::string ATTR_IMG_W = "img_w";
const std::string ATTR_NET_H = "net_h";
const std::string ATTR_NET_W = "net_w";

const std::string ATTR_NAME_TMULTIPLES = "t_multiples";

const std::string ATTR_NAME_MULTIPLES = "multiples";

const std::string ATTR_NAME_T = "T";
const std::string ATTR_NAME_N = "N";

const std::string ATTR_NAME_TSHAPE = "Tshape";
const std::string ATTR_NAME_NAN_OPT = "nan_opt";

const std::string ATTR_NAME_AIPP = "aipp";

const std::string ATTR_NAME_INPUT_FORMAT = "input_format";
const std::string ATTR_NAME_OUTPUT_FORMAT = "output_format";

const std::string ATTR_NAME_FRAMEWORK_NODE_DEF = "node_def";
const std::string ATTR_NAME_FRAMEWORK_OP_DEF = "op_def";
const std::string ATTR_NAME_FRAMEWORK_FWK_TYPE = "framework_type";
const std::string ATTR_NAME_FRAMEWORK_FUNC_DEF = "func_def";

const std::string ATTR_NAME_INPUT_TENSOR_DESC = "input_tensor_desc";
const std::string ATTR_NAME_OUTPUT_TENSOR_DESC = "output_tensor_desc";

const std::string ATTR_NAME_INFERRED_FORMAT = "inferred_format";
const std::string ATTR_NAME_PRED_PERMUTE_DELETED = "pred_permute_deleted";
const std::string ATTR_NAME_IGNORE_PRED_FORMAT = "ignore_pred_format";
const std::string ATTR_NAME_WEIGHTS = "value";
const std::string ATTR_NAME_WEIGHTS_DATA = "weights_data";
const std::string ATTR_NAME_BROACAST_REAL_DIM_CNT = "broacast_real_dim_cnt";
const std::string ATTR_NAME_DIM_ALIGN = "dim_align";
const std::string ATTR_NAME_FRAMEWORK_ORIGINAL_TYPE = "original_type";

const std::string ATTR_NAME_SESSION_GRAPH_ID = "_session_graph_id";

const std::string ATTR_NAME_AUTOMIC_ADD_START = "automic_add_addr_start";
const std::string ATTR_NAME_AUTOMIC_ADD_MEM_SIZE = "automic_add_mem_size";
const std::string ATTR_MODEL_BATCH_NUM = "batch_num";
const std::string ATTR_NAME_STREAM_LABEL = "_stream_label";
const std::string ATTR_NAME_STREAM_CYCLE_EVENT_FLAG = "need_stream_cycle_event";

// To be deleted
const std::string ATTR_TO_BE_DELETED = "to_be_deleted";
const std::string PERMUTE_RESHAPE_FUSION = "permute_reshape_fusion";
const std::string PERMUTE_RESHAPE_FUSION_CONV_PROPOSAL = "fusion_conv_proposal";
const std::string PERMUTE_RESHAPE_FUSION_CONV_DECODEBBOX = "fusion_conv_decodebbox";
const std::string PERMUTE_RESHAPE_FUSION_BOX_TYPE_NUM = "box_type_num";
const std::string SSD_MBOX_LOC_FUSION = "permute_flatten_fusion";
const std::string SSD_MBOX_CONF_FUSION = "permute_flatten_reshape_flatten_fusion";
const std::string SSD_MBOX_OCR_FUSION = "permute_flatten_ocr_fusion";
const std::string SSD_MBOX_FUSION_BOX_TYPE_NUM = "ssd_mbox_fusion_box_type_num";
const std::string SSD_RESHAPE_SLICE_CONCAT_FUSION = "reshape_slice_concat_fusion";

const std::string SSD_PRIORBOX_CONCAT = "ssd_mbox_conf_priorbox_concat_flag";

// Refinedet
const std::string REFINEDET_MBOX_LOC_FUSION = "permute_flatten_fusion";
const std::string REFINEDET_RESHAPE_SLICE_CONCAT_FUSION = "reshape_slice_concat_fusion";
const std::string REFINEDET_MBOX_CONF_FUSION = "permute_flatten_reshape_flatten_fusion";
const std::string REFINEDET_MBOX_FUSION_BOX_TYPE_NUM = "ssd_mbox_fusion_box_type_num";
const std::string REFINEDET_PRIOR_BOX_ATTR_VARIANCE = "variance";
const std::string REFINEDET_PRIOR_BOX_ATTR_VARIANCE_NUM = "variance_num";

// _Arg
const std::string ATTR_NAME_INDEX = "index";
// _RetVal
const std::string RETVAL_ATTR_NAME_INDEX = "retval_index";
// Data
const std::string DATA_ATTR_NAME_DATA_TYPE = "data_type";

// Send
const std::string SEND_ATTR_EVENT_ID = "event_id";

// Recv
const std::string RECV_ATTR_EVENT_ID = "event_id";

// convolution
const std::string ATTR_NAME_COEF = "coef";

const std::string ATTR_NAME_STRIDE = "stride";

const std::string ATTR_NAME_STRIDES = "stride";

const std::string ATTR_NAME_DILATION = "dilation";

const std::string ATTR_NAME_DILATIONS = "dilation";

const std::string CONV_ATTR_NAME_MODE = "mode";

const std::string CONV_ATTR_NAME_ALGO = "algo";

const std::string CONV_ATTR_NAME_GROUP = "group";

const std::string CONV_ATTR_NAME_PAD_MODE = "pad_mode";

const std::string CONV_ATTR_NAME_PAD = "pad";

const std::string CONV_ATTR_NAME_STRIDE = "stride";

const std::string CONV_ATTR_NAME_DILATION = "dilation";

const std::string CONV_ATTR_NAME_NUM_OUTPUT = "num_output";

const std::string CONV_ATTR_NAME_KERNEL = "kernel";

const std::string CONV_ATTR_NAME_FILTER = "filter";

const std::string CONV_ATTR_NAME_BIAS = "bias";

const std::string CONV_ATTR_NAME_RELU_FLAG = "relu_flag";

const std::string CONV_ATTR_NAME_ADJ = "adj";

const std::string CONV_ATTR_NAME_TARGET_SHAPE = "target_shape";

const std::string CONV_ATTR_NAME_BEFORE_PAD = "before_pad";

const std::string CONV_ATTR_NAME_HAS_BIAS = "has_bias";

const std::string NEED_INFER = "isNeedInfer";

// Pooling
const std::string POOLING_ATTR_MODE = "mode";
const std::string POOLING_ATTR_NAN_OPT = "nan_opt";
const std::string POOLING_ATTR_PAD_MODE = "pad_mode";
const std::string POOLING_ATTR_GLOBAL_POOLING = "global_pooling";
const std::string POOLING_ATTR_WINDOW = "window";
const std::string POOLING_ATTR_PAD = "pad";
const std::string POOLING_ATTR_STRIDE = "stride";
const std::string POOLING_ATTR_CEIL_MODE = "ceil_mode";
const std::string POOLING_ATTR_DATA_MODE = "data_mode";
const std::string POOLING_ATTR_BEFORE_PAD = "before_pad";
const std::string POOLING_ATTR_NAME_ALGO = "algo";

// Eltwise
const std::string ELTWISE_ATTR_MODE = "mode";
const std::string ELTWISE_ATTR_COEFF = "coeff";
const std::string ELTWISE_ATTR_WEIGHT = "weight";
const std::string ELTWISE_ATTR_RELU_FLAG = "relu_flag";
const std::string ELTWISE_ATTR_ALPHA = "alpha";
const std::string ELTWISE_ATTR_BETA = "beta";

// BatchNorm
const std::string BATCHNORM_ATTR_MODE = "mode";
const std::string BATCHNORM_ATTR_EPSILON = "epsilon";
const std::string BATCHNORM_ATTR_USE_GLOBAL_STATS = "use_global_stats";
const std::string BATCHNORM_ATTR_MOVING_AVERAGE_FRACTION = "moving_average_fraction";
const std::string BATCHNORM_ATTR_ESTIMATED_MEAN = "estimated_mean";
const std::string BATCHNORM_ATTR_ESTIMATED_VARIANCE = "estimated_variance";
const std::string BATCHNORM_ATTR_SCALE = "scale";
const std::string BATCHNORM_ATTR_BIAS = "bias";

// Scale
const std::string SCALE_ATTR_SCALE = "scale";
const std::string SCALE_ATTR_BIAS = "bias";

// FullConnection
const std::string FULL_CONNECTION_ATTR_FILTER = "filter";
const std::string FULL_CONNECTION_ATTR_BIAS = "bias";
const std::string FULL_CONNECTION_ATTR_NUM_OUTPUT = "num_output";
const std::string FULL_CONNECTION_ATTR_RELU_FLAG = "relu_flag";
const std::string FULL_ATTR_NAME_ALGO = "algo";

// SoftmaxOpParams
const std::string SOFTMAX_ATTR_ALGO = "algo";
const std::string SOFTMAX_ATTR_MODE = "mode";

// SparseSoftmaxCrossEntropy
const std::string SPARSE_SOFTMAX_CROSS_ENTROPY_ATTR_MODE = "cross_entropy_mode";
const std::string SPARSE_SOFTMAX_CROSS_ENTROPY_IS_GRAD = "cross_entropy_is_grad";
// Attr labelSmoothing
const std::string SOFTMAX_CROSS_ENTROPY_LABELSMOOTHING = "labelSmoothing";

// ApplyMomentum
const std::string APPLYMENTUM_ATTR_IS_GRAPH_FUSION = "applymomentum_is_graph_fusion";

// Activation
const std::string ACTIVATION_ATTR_MODE = "mode";
const std::string ACTIVATION_ATTR_COEF = "coef";

// Concat
const std::string CONCAT_ATTR_NAME_AXIS = "axis";

// Const
const std::string CONST_ATTR_NAME_DATA_TRANSTYPE = "data_transtype";
const std::string CONST_ATTR_NAME_OUTPUT_FORMAT = "output_format";
const std::string CONST_ATTR_NAME_OUTPUT_TYPE = "output_type";

// Roipooling
const std::string ROIPOOLING_ATTR_NAME_POOLED_H = "pooled_h";
const std::string ROIPOOLING_ATTR_NAME_POOLED_W = "pooled_w";
const std::string ROIPOOLING_ATTR_NAME_SPATIAL_SCALE = "spatial_scale";
const std::string ROIPOOLING_ATTR_NAME_RIO_POOLING_MODE = "rio_pooling_mode";
const std::string ROIPOOLING_ATTR_NAME_POOLING_MODE = "pooling_mode";
const std::string ROIPOOLING_ATTR_NAME_SAMPLING_RATIO = "sampling_ratio";

// DetectionOutput
const std::string DETECTIONOUTPUT_ATTR_NUM_CLASSES = "num_classes";
const std::string DETECTIONOUTPUT_ATTR_OCR_NUM_CLASSES = "ocr_num_classes";
const std::string DETECTIONOUTPUT_ATTR_NMS_THRESHOLD = "nms_threshold";
const std::string DETECTIONOUTPUT_ATTR_TOP_K = "top_k";
const std::string DETECTIONOUTPUT_ATTR_CONFIDENCE_THRESHOLD = "confidence_threshold";
const std::string DETECTIONOUTPUT_ATTR_IMG_H = "img_h";
const std::string DETECTIONOUTPUT_ATTR_IMG_W = "img_w";
const std::string DETECTIONOUTPUT_ATTR_BATCH_SIZE = "batch_size";
// Ssd DetectionOutput
const std::string DETECTIONOUTPUT_ATTR_ETA = "eta";
const std::string DETECTIONOUTPUT_ATTR_SHARED_LOCATION = "shared_location";
const std::string DETECTIONOUTPUT_ATTR_BACKGROUND_LABEL_ID = "background_label_id";
const std::string DETECTIONOUTPUT_ATTR_CODE_TYPE = "code_type";
const std::string DETECTIONOUTPUT_ATTR_VARIANCE_ENCODED_IN_TARGET = "variance_encoded_in_target";
const std::string DETECTIONOUTPUT_ATTR_KEEP_TOP_K = "keep_top_k";
// Refinedet DetectionOutput
const std::string DETECTIONOUTPUT_ATTR_OBJECTNESS_SCORE = "objectness_score";
// yolo DetectionOutput
const std::string DETECTIONOUTPUT_ATTR_ClASSES = "classes";
const std::string DETECTIONOUTPUT_ATTR_BIASES = "biases";
const std::string DETECTIONOUTPUT_ATTR_RELATIVE = "relative";
const std::string DETECTIONOUTPUT_ATTR_OBJECTNESS_THRESHOLD = "objectness_threshold";
const std::string DETECTIONOUTPUT_ATTR_CLASS_THRESHOLD = "class_threshold";
const std::string DETECTIONOUTPUT_ATTR_POST_TOP_K = "post_top_k";
const std::string DETECTIONOUTPUT_ATTR_IOU_THRESHOLD_DECAY = "iou_threshold_decay";
const std::string DETECTIONOUTPUT_ATTR_COOR_SCALE_FACTOR = "coor_scale_factor";
const std::string DETECTIONOUTPUT_ATTR_YOLO_VERSION = "yolo_version";

// DetectionPostprocess
const std::string POSTPROCESS_ATTR_NAME_CLS_NUM = "cls_num";
const std::string POSTPROCESS_ATTR_NAME_CONF_THRESH = "conf_thresh";
const std::string POSTPROCESS_ATTR_NAME_NMS_THRESH = "nms_thresh";
const std::string POSTPROCESS_ATTR_POST_NMS_TOPN = "post_nms_topn";
const std::string POSTPROCESS_ATTR_NAME_BBOX_REG_WEIGHT = "bbox_reg_weights";

// Spatialtransfrom
const std::string SPTIALTF_ATTR_NAME_OUTPUT_H = "output_h";
const std::string SPTIALTF_ATTR_NAME_OUTPUT_W = "output_w";
const std::string SPTIALTF_ATTR_NAME_BORDER_VALUE = "border_value";
const std::string SPTIALTF_ATTR_NAME_AFFINE_TRANSFORM = "affine_transform";

// Proposa
const std::string PROPOSAL_ATTR_NAME_FEAT_STRIDE = "feat_stride";
const std::string PROPOSAL_ATTR_NAME_BASE_SIZE = "base_size";
const std::string PROPOSAL_ATTR_NAME_MIN_SIZE = "min_size";
const std::string PROPOSAL_ATTR_NAME_RATIO = "ratio";
const std::string PROPOSAL_ATTR_NAME_SCALE = "scale";
const std::string PROPOSAL_ATTR_NAME_PRE_NMS_TOPN = "pre_nms_topn";
const std::string PROPOSAL_ATTR_NAME_POST_NMS_TOPN = "post_nms_topn";
const std::string PROPOSAL_ATTR_NAME_NMS_THRESH = "nms_thresh";
const std::string PROPOSAL_ATTR_NAME_TOP_SIZE = "top_size";
const std::string PROPOSAL_ATTR_IMG_H = "img_h";
const std::string PROPOSAL_ATTR_IMG_W = "img_w";
// Softmax
const std::string SOFTMAX_ATTR_AXIS = "axis";

// Permute
const std::string PERMUTE_ATTR_ORDER = "order";

// SSD Normalize
const std::string SSDNORMALIZE_ATTR_ACCROSS_SPATIAL = "across_spatial";
const std::string SSDNORMALIZE_ATTR_CHANNEL_SHARED = "channel_shared";
const std::string SSDNORMALIZE_ATTR_EPS = "eps";

// Flatten
const std::string FLATTEN_ATTR_AXIS = "axis";
const std::string FLATTEN_ATTR_END_AXIS = "end_axis";

// SsdPRIORBOX
const std::string SSD_PRIOR_BOX_ATTR_FLIP = "flip";
const std::string SSD_PRIOR_BOX_ATTR_CLIP = "clip";
const std::string SSD_PRIOR_BOX_ATTR_IMG_H = "img_h";
const std::string SSD_PRIOR_BOX_ATTR_IMG_W = "img_w";
const std::string SSD_PRIOR_BOX_ATTR_STEP_H = "step_h";
const std::string SSD_PRIOR_BOX_ATTR_STEP_W = "step_w";
const std::string SSD_PRIOR_BOX_ATTR_OFFSET = "offset";
const std::string SSD_PRIOR_BOX_ATTR_MIN_SIZE = "min_size";
const std::string SSD_PRIOR_BOX_ATTR_MAX_SIZE = "max_size";
const std::string SSD_PRIOR_BOX_ATTR_MIN_SIZE_NUM = "min_size_num";
const std::string SSD_PRIOR_BOX_ATTR_MAX_SIZE_NUM = "max_size_num";
const std::string SSD_PRIOR_BOX_ATTR_ASPECT_RATIO = "aspect_ratio";
const std::string SSD_PRIOR_BOX_ATTR_ASPECT_RATIO_NUM = "aspect_ratio_num";
const std::string SSD_PRIOR_BOX_ATTR_VARIANCE = "variance";
const std::string SSD_PRIOR_BOX_ATTR_VARIANCE_NUM = "variance_num";

// PRelu
const std::string PRELU_ATTR_CHANNEL_SHARED = "channel_shared";

// Psroi pooling
const std::string PSROIPOOLING_ATTR_SPATIAL_SCALE = "spatial_scale";
const std::string PSROIPOOLING_ATTR_OUTPUT_DIM = "output_dim";
const std::string PSROIPOOLING_ATTR_GROUP_SIZE = "group_size";

// Power
const std::string POWER_ATTR_NAME_POWER = "power";
const std::string POWER_ATTR_NAME_SCALE = "scale";
const std::string POWER_ATTR_NAME_SHIFT = "shift";

// Pack
const std::string PACK_ATTR_NAME_NUM = "N";

// Unpack
const std::string UNPACK_ATTR_NAME_NUM = "num";
// Gathernd
const std::string GATHERND_ATTR_NAME_TINDICES = "Tindices";
const std::string GATHERND_ATTR_NAME_TPARAMS = "Tparams";

// Argmax
const std::string ARGMAX_ATTR_NAME_TOPK = "topk";
const std::string ARGMAX_ATTR_NAME_REDUCESIZE = "reduce_size";
const std::string ARGMAX_ATTR_NAME_REDUCESTRIDE = "reduce_stride";
const std::string ARGMAX_ATTR_NAME_OUTMAX = "outmaxval";

// Relu
const std::string ATTR_NAME_NEGATIVE_SLOPE = "negative_slope";

// FreeSpaceExtract
const std::string FREESPACEEXTRACT_ATTR_NAME_ORG_HEIGHT = "org_height";

// Split
const std::string SPLIT_ATTR_NAME_SLICE_POINT = "slice_point";
const std::string SPLIT_ATTR_NAME_SIZE_SPLIT = "size_split";
const std::string SPLIT_ATTR_NAME_NUM_SPLIT = "num_split";

// Tvm
const std::string TVM_ATTR_NAME_MAGIC = "tvm_magic";
const std::string TVM_ATTR_NAME_BLOCKDIM = "tvm_blockdim";
const std::string TVM_ATTR_NAME_METADATA = "tvm_metadata";

// Squeeze
const std::string SQUEEZE_ATTR_AXIS = "axis";
const std::string SQUEEZE_ATTR_DIMS = "squeeze_dims";
const std::string SQUEEZE_OP_NAME = "Squeeze";

// Stride slice
const std::string STRIDE_SLICE_ATTR_BEGIN_MASK = "begin_mask";
const std::string STRIDE_SLICE_ATTR_END_MASK = "end_mask";
const std::string STRIDE_SLICE_ATTR_ELLIPSIS_MASK = "ellipsis_mask";
const std::string STRIDE_SLICE_ATTR_NEW_AXIS_MASK = "new_axis_mask";
const std::string STRIDE_SLICE_ATTR_SHRINK_AXIS_MASK = "shrink_axis_mask";

// Slice
const std::string SLICE_ATTR_NAME_BEGINS = "begins";
const std::string SLICE_ATTR_NAME_SIZES = "sizes";

// Roialign
const std::string ROIALIGN_ATTR_SPATIAL_SCALE = "spatial_scale";
const std::string ROIALIGN_ATTR_SAMPLING_RATIO = "sampling_ratio";
const std::string ROIALIGN_ATTR_NAME_POOLED_H = "pooled_h";
const std::string ROIALIGN_ATTR_NAME_POOLED_W = "pooled_w";

// Generate_rpn_proposal
const std::string GENERATE_RPN_PROPOSAL_ATTR_PRE_NMS_TOPK = "pre_nms_topk";
const std::string GENERATE_RPN_PROPOSAL_ATTR_POST_NMS_TOPK = "post_nms_topk";
const std::string GENERATE_RPN_PROPOSAL_ATTR_RPN_MINI_SIZE = "rpn_mini_size";
const std::string GENERATE_RPN_PROPOSAL_ATTR_RPN_PROPOSAL_NMS_THRESH = "rpn_proposal_nms_thresh";
const std::string GENERATE_RPN_PROPOSAL_ATTR_RPN_PROPOSAL_FILTER_THRESH = "rpn_proposal_filter_thresh";
// Decode_bbox
const std::string DECODE_BBOX_ATTR_DECODECLIP = "decodeClip";

// Cast
const std::string CAST_ATTR_DSTT = "DstT";
const std::string CAST_ATTR_SRCT = "SrcT";
const std::string CAST_ATTR_DST_TYPE = "dst_type";
const std::string CAST_ATTR_TRUNCATE = "truncate";

// Fastrcnnn predications
const std::string FASTRCNN_PREDICTIONS_ATTR_TOPK = "fsr_topk";
const std::string FASTRCNN_PREDICTIONS_ATTR_SCORE_THRESHOLD = "fsr_score_thres";
const std::string FASTRCNN_PREDICTIONS_ATTR_NMS_THRESHOLD = "fsr_nms_thres";
const std::string FASTRCNN_PREDICTIONS_ATTR_NUM_CLASSES = "fsr_num_classes";

// REORG
const std::string REORG_ATTR_STRIDE = "stride";
const std::string REORG_ATTR_REVERSE = "reverse";

// MERGE
const std::string MERGE_DEAD_INDEX = "merge_dead_index";
const std::string MERGE_PRENODE_FLAG = "merge_prenode_flag";
const std::string TO_BE_OUTPUT = "to_be_output";

// ENTER
const std::string ENTER_ATTR_FRAME_NAME = "frame_name";
const std::string ENTER_ATTR_CONSTANT_FLAG = "is_constant";

// Concatv2
const std::string CONCAT_V2_ATTR_TIDX = "Tidx";
const std::string CONCAT_V2_ATTR_N = "N";
// SUM
const std::string SUM_ATTR_TIDX = "Tidx";
const std::string SUM_ATTR_AXIS = "axis";
const std::string SUM_ATTR_KEEP_DIMS = "keep_dims";

// ResizeBilinear
const std::string RESIZE_BILINEAR_ATTR_MODE = "mode";
const std::string RESIZE_BILINEAR_ATTR_ALIGN_CORNERS = "align_corners";
const std::string RESIZE_BILINEAR_ATTR_HEIGHT = "height";
const std::string RESIZE_BILINEAR_ATTR_WIDTH = "width";
const std::string RESIZE_BILINEAR_ATTR_ZOOM_FACTOR = "zoom_factor";
const std::string RESIZE_BILINEAR_ATTR_SHRINK_FACTOR = "shrink_factor";
const std::string RESIZE_BILINEAR_ATTR_PAD_BEGIN = "pad_begin";
const std::string RESIZE_BILINEAR_ATTR_PAD_END = "pad_end";
const std::string RESIZE_BILINEAR_ATTR_ALPHA = "alpha";
const std::string RESIZE_BILINEAR_ATTR_BETA = "beta";

// RetinaNet
const std::string RETINANET_FILTER_BACKGROUND_TRUE = "retina_conv_filter_background";
const std::string RETINANET_ANCHOR_FUSION = "retina_anchor_fusion";

// MatMul
const std::string MATMUL_TRANSPOSE_X = "transposeX";
const std::string MATMUL_TRANSPOSE_W = "transposeW";
const std::string MATMUL_HAS_BIAS = "has_bias";
const std::string MATMUL_ATTR_IS_TRAINING = "matmul_is_training";

// Flatten
const std::string FLATTEN_START_AXIS = "start_axis";
const std::string FLATTEN_END_AXIS = "end_axis";

// Reshape
const std::string RESHAPE_ATTR_AXIS = "axis";
const std::string RESHAPE_ATTR_NUM_AXES = "num_axes";
const std::string RESHAPE_ATTR_FORMAT = "format";
const std::string RESHAPE_ATTR_SHAPE = "shape";
const std::string RESHAPE_ATTR_ALPHA = "alpha";
const std::string RESHAPE_ATTR_BETA = "beta";

// Frameoworkop
const std::string T_IN_DATATYPE = "t_in_datatype";
const std::string T_OUT_DATATYPE = "t_out_datatype";
const std::string ATTR_NAME_OUT_N = "out_n";
const std::string ATTR_NAME_OUT_C = "out_c";
const std::string ATTR_NAME_OUT_H = "out_h";
const std::string ATTR_NAME_OUT_W = "out_w";
const std::string ATTR_PAD_DEPTH_CONV = "pad_depth_conv";
const std::string ATTR_PAD_CONV = "pad_conv";

const std::string ATTR_NAME_BEFORE_PAD = "before_pad";
const std::string ANN_MEAN_KEEPDIMS = "AnnMeanKeepDims";
const std::string PAD_ATTR_PADDINGDS = "paddings";
const std::string PAD_ATTR_CONSTANT_VALUE = "padvalue";

// ConvGradFilter
const std::string CONV_GRAD_FILTER_OUTPUT_SHAPE = "conv_grad_filter_output_shape";
// ConvGradInput
const std::string CONV_GRAD_INPUT_OUTPUT_SHAPE = "conv_grad_input_output_shape";

// Rnn
const std::string RNN_MODE_ = "rnn_";
const std::string CNN_RNN = "cnn_rnn";
const std::string MUTI_RNN = "multi_rnn";
const std::string CELL_MODE = "mode";
const std::string LSTM_CELL = "lstm_cell";
const std::string GRU_CELL = "gru_cell";
const std::string RNN_HT = "ht";
const std::string RNN_XT_HT = "xt_ht";
const std::string RNN_BATCH_SIZE = "batch_size";

// Upsample
const std::string UPSAMPLE_ATTR_NAME_SCALE = "scale";

// Filler
const std::string FILLER_TYPE = "filler_type";
const std::string FILLER_VALUE = "filler_value";

// Shufflechannel
const std::string SHUFFLE_CHANNEL_GROUP = "group";

// TopKV2
const std::string TOPKV2_ATTR_K = "k";

const std::string DEPTH_SPACE_ATTR_BLOCK_SIZE = "block_size";
const std::string L2_NORMALIZE_ATTR_EPS = "eps";

// Calibaration
const std::string STRIDE_H_INDEX = "STRIDE_H_INDEX";
const std::string STRIDE_W_INDEX = "STRIDE_W_INDEX";
const std::string PAD_TOP_INDEX = "PAD_TOP_INDEX";
const std::string PAD_BOTTOM_INDEX = "PAD_BOTTOM_INDEX";
const std::string PAD_RIGHT_INDEX = "PAD_RIGHT_INDEX";
const std::string PAD_LEFT_INDEX = "PAD_LEFT_INDEX";
const std::string QUANTIZE_ALGO_ATTR = "quantize_algo";
const std::string SCALE_TYPE_ATTR = "scale_type";

const std::string QUANTIZE_SCALE_MODE = "quantize_scale_mode";
const std::string QUANTIZE_SCALE_VALUE = "quantize_scale_value";
const std::string QUANTIZE_SCALE_OFFSET = "quantize_scale_offset";
const std::string QUANTIZE_OFFSET_DATA_VALUE = "quantize_offset_data_value";
const std::string QUANTIZE_OFFSET_DATA_OFFSET = "quantize_offset_data_offset";
const std::string QUANTIZE_OFFSET_WEIGHT_VALUE = "quantize_offset_weight_value";
const std::string QUANTIZE_OFFSET_WEIGHT_OFFSET = "quantize_offset_weight_offset";
const std::string QUANTIZE_OFFSET_PAD_VALUE = "quantize_offset_pad_value";
const std::string QUANTIZE_OFFSET_PAD_OFFSET = "quantize_offset_pad_offset";

const std::string DEQUANTIZE_SCALE_MODE = "dequantize_scale_mode";
const std::string DEQUANTIZE_SCALE_VALUE = "dequantize_scale_value";
const std::string DEQUANTIZE_SCALE_OFFSET = "dequantize_scale_offset";
const std::string DEQUANTIZE_OFFSET_DATA_TYPE = "dequantize_offset_data_value";
const std::string DEQUANTIZE_OFFSET_DATA_OFFSET = "dequantize_offset_data_offset";
const std::string DEQUANTIZE_OFFSET_WEIGHT_VALUE = "dequantize_offset_weight_value";
const std::string DEQUANTIZE_OFFSET_WEIGHT_OFFSET = "dequantize_offset_weight_offset";
const std::string DEQUANTIZE_OFFSET_PAD_VALUE = "dequantize_offset_pad_value";
const std::string DEQUANTIZE_OFFSET_PAD_OFFSET = "dequantize_offset_pad_offset";

const std::string REQUANTIZE_SCALE_MODE = "requantize_scale_mode";
const std::string REQUANTIZE_SCALE_VALUE = "requantize_scale_value";
const std::string REQUANTIZE_SCALE_OFFSET = "requantize_scale_offset";
const std::string REQUANTIZE_OFFSET_DATA_VALUE = "requantize_offset_data_value";
const std::string REQUANTIZE_OFFSET_DATA_OFFSET = "requantize_offset_data_offset";
const std::string REQUANTIZE_OFFSET_WEIGHT_VALUE = "requantize_offset_weight_value";
const std::string REQUANTIZE_OFFSET_WEIGHT_OFFSET = "requantize_offset_weight_offset";
const std::string REQUANTIZE_OFFSET_PAD_VALUE = "requantize_offset_pad_value";
const std::string REQUANTIZE_OFFSET_PAD_OFFSET = "requantize_offset_pad_offset";

const std::string ATTR_NAME_IS_CONST = "attr_name_is_const";

const std::string ATTR_NAME_GROUP = "group";
const std::string ATTR_NAME_DILATION_SIZE = "dilation_size";
const std::string ATTR_NAME_EPSILON = "epsilon";
const std::string ATTR_NAME_POOLING_MODE = "mode";
const std::string ATTR_NAME_CLASS_NUM = "class_num";
// model
const std::string ATTR_MODEL_TARGET_TYPE = "target_type";

const std::string ATTR_MODEL_STREAM_NUM = "stream_num";

const std::string ATTR_MODEL_EVENT_NUM = "event_num";

const std::string ATTR_MODEL_MEMORY_SIZE = "memory_size";

const std::string ATTR_MODEL_WEIGHT_SIZE = "weight_size";

const std::string ATTR_MODEL_TASK_GEN_BASE_ADDR = "task_gen_base_addr";

const std::string ATTR_MODEL_TASK_GEN_WEIGHT_ADDR = "task_gen_weight_addr";

const std::string ATTR_MODEL_TASK_GEN_VAR_ADDR = "task_gen_variable_addr";

const std::string ATTR_MODEL_VAR_SIZE = "variable_size";

const std::string ATTR_MODEL_TASK_INDEX_OP_NAME = "task_index_op_name";

// Public attribute
const std::string ATTR_NAME_IMPLY_TYPE = "imply_type";

const std::string ATTR_NAME_BYTE_SIZE = "op_byte_size";

const std::string ATTR_NAME_FUSION_INFERENCE_ID = "fusion_inference_id";

const std::string ATTR_NAME_FUSION_OPDEF = "fusion_opdef";

const std::string ATTR_NAME_IO_OP = "io_op";

const std::string ATTR_NAME_FUSION_SCOPE = "fusion_scope";

const std::string ATTR_NAME_OPATTR = "opattr";

const std::string ATTR_NAME_RELUFLAG = "relu_flag";

const std::string ATTR_NAME_SEQLEN_INDEX = "seqlen_index";

const std::string ATTR_NAME_X_INDEX = "x_index";

const std::string ATTR_NAME_CONT_INDEX = "cont_index";

const std::string ATTR_NAME_XSTATIC_INDEX = "xstatic_index";

const std::string TARGET_TYPE_MINI = "MINI";

const std::string TARGET_TYPE_TINY = "TINY";

const std::string TARGET_TYPE_LITE = "LITE";

const std::string ATTR_NAME_CONTINUOUS_INPUT = "continuous_input";

const std::string ATTR_NAME_CONTINUOUS_OUTPUT = "continuous_output";

const std::string ATTR_NAME_REFERENCE = "reference";

const std::string ATTR_NAME_ATOMIC_INDEX = "atomic_index";

// Used for mark the active label list stream of activated node
const std::string ATTR_NAME_ACTIVE_LABEL_LIST = "_active_label_list";

// Multi batch
const std::string ATTR_NAME_PRED_VALUE = "_pred_value";
const std::string ATTR_NAME_BATCH_NUM = "_batch_num";
const std::string ATTR_NAME_BATCH_LABEL = "_batch_label";

// Control flow
const std::string ATTR_NAME_STREAM_SWITCH_COND = "switch_condition";
const std::string ATTR_NAME_TRUE_BRANCH_STREAM = "true_branch_stream";
const std::string ATTR_NAME_ACTIVE_STREAM_LIST = "active_stream_list";
const std::string ATTR_NAME_SWITCHN_PRED_VALUE = "switch_pred_value";

const std::string ATTR_NAME_SWITCH_BRANCH_NODE_LABEL = "_switch_branch_node_label";
const std::string ATTR_NAME_SWITCH_TRUE_BRANCH_FLAG = "_switch_true_branch_flag";
const std::string ATTR_NAME_SWITCH_DATA_TYPE = "_switch_data_type";
const std::string ATTR_NAME_ORIG_NODE_NAME = "_original_node_name";
const std::string ATTR_NAME_CYCLIC_DEPENDENCE_FLAG = "_cyclic_dependence_flag";

const std::string ATTR_NAME_NEXT_ITERATION = "_next_iteration_node";

// Used for mark the active node is for loop, type:bool
const std::string ATTR_NAME_IS_LOOP_ACTIVE = "is_loop_active";

const std::string ATTR_NAME_MEMORY_TYPE_INPUT = "memory_type_input";

const std::string ATTR_NAME_MEMORY_TYPE_OUTPUT = "memory_type_output";

const std::string ATTR_NAME_MEMORY_TYPE_WORKSPACE = "memory_type_workspace";

const std::string MODEL_ATTR_SESSION_ID = "session_id";

// Atomic addr clean attrs
const std::string ATOMIC_ATTR_INPUT_INDEX = "atomic_input_index";
const std::string ATOMIC_ATTR_OUTPUT_INDEX = "atomic_output_index";
const std::string ATOMIC_ATTR_IS_FUSION_NODE = "is_fusion_node";
const std::string EXT_ATTR_ATOMIC_WORKSPACE_INFO = "sub_node_workspace_info";
const std::string EXT_ATTR_ATOMIC_WORKSPACE_OFFSET = "sub_node_workspace_offset";
const std::string ATOMIC_ATTR_IS_ATOMIC_NODE = "is_atomic_node";

// Source/dst format for Op FormatTransfer
const std::string FORMAT_TRANSFER_SRC_FORMAT = "src_format";
const std::string FORMAT_TRANSFER_DST_FORMAT = "dst_format";

// For compile op by ge call
const std::string ATTR_NEED_COMPILE = "_node_need_compile";

const std::string ATTR_INSERT_BY_MBATCH = "mbatch-inserted-node";

// For inserted op
const std::string ATTR_INSERTED_BY_GE = "_inserted_by_ge";

// For data dump
const std::string ATTR_NAME_DATA_DUMP_ORIGIN_OP_NAMES = "_datadump_original_op_names";
const std::string ATTR_NAME_DATA_DUMP_IS_MULTIOP = "_datadump_is_multiop";
const std::string ATTR_NAME_DATA_DUMP_SUB_SPLITER_INDEX = "_datadump_sub_spliter_index";
const std::string ATTR_NAME_DATA_DUMP_GROUP_OP_NAME = "_datadump_group_op_name";
const std::string ATTR_NAME_DATA_DUMP_ORIGIN_NAME = "_datadump_origin_name";
const std::string ATTR_NAME_DATA_DUMP_ORIGIN_OUTPUT_INDEX = "_datadump_origin_output_index";
const std::string ATTR_NAME_DATA_DUMP_ORIGIN_FORMAT = "_datadump_origin_format";
const std::string ATTR_NAME_DATA_DUMP_ORIGIN_DATA_TYPE = "_datadump_origin_data_type";

// Variable
const std::string REF_VAR_SRC_VAR_NAME = "ref_var_src_var_name";
const std::string VAR_ATTR_SRC_VAR_NAME = "_src_var_name";
const std::string REF_VAR_PRE_PEER_OUT_INDEX = "ref_var_pre_peer_out_index";
const std::string VAR_ATTR_VAR_IS_BROADCAST = "_var_is_broadcast";
const std::string VAR_ATTR_VAR_IS_RESTORE = "_var_is_restore";

// HCOM
const std::string HCOM_ATTR_ROOT_RANK = "root_rank";
const std::string HCOM_ATTR_RANK_SIZE = "rank_size";
const std::string HCOM_ATTR_SHAPE = "shape";
const std::string HCOM_ATTR_DATA_TYPE = "dtype";

const std::string HCOM_ATTR_REDUCE_TYPE = "reduction";

const std::string ATTR_NAME_INPUT_DATATYPE = "input_datatype";
const std::string ATTR_NAME_OUTPUT_DATATYPE = "output_datatype";

// Dynamic stitch
const std::string DYNAMIC_STITCH_ATTR_NAME_NUM = "DynamicStitchN_";
}  // namespace ge
