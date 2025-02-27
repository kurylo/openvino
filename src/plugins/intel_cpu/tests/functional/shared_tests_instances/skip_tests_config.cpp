// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "functional_test_utils/skip_tests_config.hpp"

#include <ie_system_conf.h>

#include <string>
#include <vector>

#include "ie_parallel.hpp"

std::vector<std::string> disabledTestPatterns() {
    std::vector<std::string> retVector{
        // TODO: Issue 31841
        R"(.*(QuantGroupConvBackpropData3D).*)",
        // TODO: Issue 31843
        R"(.*(QuantConvBackpropData3D).*)",
        R"(.*(QuantConvBackpropData2D).*(QG=Perchannel).*)",
        R"(.*(QuantGroupConvBackpropData2D).*(QG=Perchannel).*)",
        // TODO: Issue 33886
        R"(.*(QuantGroupConv2D).*)",
        R"(.*(QuantGroupConv3D).*)",
        // TODO: Issue: 34518
        R"(.*RangeLayerTest.*)",
        R"(.*(RangeAddSubgraphTest).*Start=1.2.*Stop=(5.2|-5.2).*Step=(0.1|-0.1).*netPRC=FP16.*)",
        R"(.*(RangeNumpyAddSubgraphTest).*netPRC=FP16.*)",
        // TODO: Issue: 43793
        R"(.*InferRequestPreprocessDynamicallyInSetBlobTest.*iPRC=0.*_iLT=1.*)",
        R"(.*InferRequestPreprocessDynamicallyInSetBlobTest.*oPRC=0.*_oLT=1.*)",
        // TODO: Issue: 63469
        R"(.*ConversionLayerTest.*ConvertLike.*)",
        // TODO: Issue: 34055
        R"(.*ReluShapeOfSubgraphTest.*)",
        // TODO: Issue: 43314
        R"(.*Broadcast.*mode=BIDIRECTIONAL.*inNPrec=BOOL.*)",
        // TODO: Issue 43417 sporadic issue, looks like an issue in test, reproducible only on Windows platform
        R"(.*decomposition1_batch=5_hidden_size=10_input_size=30_.*tanh.relu.*_clip=0_linear_before_reset=1.*_targetDevice=CPU_.*)",
        // Skip platforms that do not support BF16 (i.e. sse, avx, avx2)
        R"(.*(BF|bf)16.*(jit_avx(?!5)|jit_sse).*)",
        // TODO: Incorrect blob sizes for node BinaryConvolution_X
        R"(.*BinaryConvolutionLayerTest.*)",
        // TODO: 53618. BF16 gemm ncsp convolution crash
        R"(.*_GroupConv.*_inFmts=nc.*_primitive=jit_gemm.*ENFORCE_BF16=YES.*)",
        // TODO: 53578. fork DW bf16 convolution does not support 3d cases yet
        R"(.*_DW_GroupConv.*_inFmts=(ndhwc|nCdhw16c).*ENFORCE_BF16=YES.*)",
        // TODO: 56143. Enable nspc convolutions for bf16 precision
        R"(.*ConvolutionLayerCPUTest.*_inFmts=(ndhwc|nhwc).*ENFORCE_BF16=YES.*)",
        // TODO: 56827. Sporadic test failures
        R"(.*smoke_Conv.+_FP32.ConvolutionLayerCPUTest\.CompareWithRefs.*TS=\(\(.\.67.+\).*inFmts=n.+c.*_primitive=jit_avx2.*)",
        // incorrect jit_uni_planar_convolution with dilation = {1, 2, 1} and output channel 1
        R"(.*smoke_Convolution3D.*D=\(1.2.1\)_O=1.*)",

        // TODO: Issue: 35627. CPU Normalize supports from 2D to 4D blobs
        R"(.*NormalizeL2_1D.*)",
        R"(.*NormalizeL2_5D.*)",
        // Issue: 59788. dnnl_normalize_nchw applies eps after sqrt for across_spatial
        R"(.*NormalizeL2_.*axes=\(1.2.*_eps=100.*)",
        R"(.*NormalizeL2_.*axes=\(2.1.*_eps=100.*)",
        R"(.*NormalizeL2_.*axes=\(3.1.2.*_eps=100.*)",

        // Unsupported operation of type: NormalizeL2 name : Doesn't support reduction axes: (2.2)
        R"(.*BF16NetworkRestore1.*)",
        R"(.*MobileNet_ssd_with_branching.*)",

        // TODO: 57562 No dynamic output shape support
        R"(.*NonZeroLayerTest.*)",
        // Not expected behavior
        R"(.*Behavior.*InferRequestIOBBlobSetLayoutTest.*layout=(SCALAR|OIHW).*)",
        R"(.*Behavior.*InferRequestIOBBlobSetLayoutTest.*CanSetOutBlobWithDifferentLayouts.*layout=HW.*)",
        R"(.*Behavior.*InferRequestIOBBlobSetLayoutTest.*CanSetInBlobWithDifferentLayouts.*layout=NHWC.*targetDevice=(AUTO|MULTI).*)",
        R"(.*Behavior.*InferRequestIOBBlobSetLayoutTest.*CanSetOutBlobWithDifferentLayouts.*layout=CN.*targetDevice=(AUTO|MULTI).*)",
        R"(.*Behavior.*InferRequestSetBlobByType.*Batched.*)",
        R"(.*Auto_Behavior.*InferRequestIOBBlobTest.*canProcessDeallocatedOutputBlobAfterGetAndSetBlob.*)",
        R"(.*Auto.*Behavior.*ExecutableNetworkBaseTest.*canLoadCorrectNetworkToGetExecutableWithIncorrectConfig.*)",
        R"(.*(Auto|Multi).*Behavior.*CorrectConfigAPITests.*CanSetExclusiveAsyncRequests.*)",
        R"(.*(Auto|Multi).*Behavior.*IncorrectConfigTests.*CanNotLoadNetworkWithIncorrectConfig.*)",
        R"(.*OVCompiledModelBaseTest.*(CanGetInputsInfoAndCheck|canSetConfigToCompiledModel).*)",
        R"(.*Behavior.*CorrectConfigCheck.*(canSetConfigAndCheckGetConfig|canSetConfigTwiceAndCheckGetConfig).*CPU_BIND_THREAD=YES.*)",
        // Issue: 72021 Unreasonable abs_threshold for comparing bf16 results
        R"(.*smoke_Reduce.*type=(Prod|Min).*netPRC=(BF|bf)16.*)",
        // TODO: 56520 Accuracy mismatch
        R"(.*ReduceOpsLayerTest.*type=Mean_.*netPRC=(I64|I32).*)",
        R"(.*ReduceOpsLayerTest.*type=Mean_.*netPRC=U64.*)",
        // Not implemented yet:
        R"(.*Behavior.*ExecutableNetworkBaseTest.*canSetConfigToExecNet.*)",
        R"(.*Behavior.*OVCompiledModelBaseTest.*canSetConfigToCompiledModel.*)",
        R"(.*(Auto|Multi).*Behavior.*ExecutableNetworkBaseTest.*checkGetExecGraphInfo.*)",
        R"(.*(Auto|Multi).*Behavior.*OVCompiledModelBaseTest.*checkGetExecGraphInfo.*)",
        R"(.*(Auto|Multi).*Behavior.*ExecutableNetworkBaseTest.*CanCreateTwoExeNetworksAndCheckFunction.*)",
        R"(.*(Auto|Multi).*Behavior.*OVCompiledModelBaseTest.*canCreateTwoCompiledModelAndCheckTheir.*)",
        R"(.*(Auto|Multi).*Behavior.*ExecutableNetworkBaseTest.*(CheckExecGraphInfoBeforeExecution|CheckExecGraphInfoAfterExecution).*)",
        R"(.*(Auto|Multi).*Behavior.*OVCompiledModelBaseTest.*(CheckExecGraphInfoBeforeExecution|CheckExecGraphInfoAfterExecution).*)",
        R"(.*(Auto|Multi).*Behavior.*ExecutableNetworkBaseTest.*CheckExecGraphInfoSerialization.*)",
        R"(.*(Auto|Multi).*Behavior.*OVCompiledModelBaseTest.*CheckExecGraphInfoSerialization.*)",
        R"(.*Behavior.*ExecutableNetworkBaseTest.*canExport.*)",
        R"(.*Behavior.*OVCompiledModelBaseTest.*canExportModel.*)",
        R"(.*Behavior.*ExecutableNetworkBaseTest.*canSetConfigToExecNetWithIncorrectConfig.*)",
        R"(.*Behavior.*OVCompiledModelBaseTest.*canSetConfigToCompiledModelWithIncorrectConfig.*)",
        R"(.*Hetero.*Behavior.*ExecutableNetworkBaseTest.*ExecGraphInfo.*)",
        R"(.*Hetero.*Behavior.*OVCompiledModelBaseTest.*ExecGraphInfo.*)",
        R"(.*Hetero.*Behavior.*ExecutableNetworkBaseTest.*CanCreateTwoExeNetworksAndCheckFunction.*)",
        // TODO: CVS-104942
        R"(.*(Auto|Multi).*Behavior.*ExecutableNetworkBaseTest.*canLoadCorrectNetworkToGetExecutableAndCheckConfig.*)",
        R"(.*(Auto|Multi).*SetPropLoadNetWorkGetPropTests.*)",
        R"(.*Hetero.*Behavior.*OVCompiledModelBaseTest.*canCreateTwoCompiledModelAndCheckTheir.*)",
        // CPU does not support dynamic rank
        // Issue: CVS-66778
        R"(.*smoke_BehaviorTests.*InferFullyDynamicNetworkWith(S|G)etTensor.*)",
        R"(.*smoke_Hetero_BehaviorTests.*InferFullyDynamicNetworkWith(S|G)etTensor.*)",
        R"(.*smoke_Auto_BehaviorTests.*InferFullyDynamicNetworkWith(S|G)etTensor.*)",
        R"(.*smoke_BehaviorTests.*DynamicOutputToDynamicInput.*)",
        R"(.*smoke_BehaviorTests.*DynamicInputToDynamicOutput.*)",
        R"(.*smoke_Hetero_BehaviorTests.*DynamicOutputToDynamicInput.*)",
        R"(.*smoke_Hetero_BehaviorTests.*DynamicInputToDynamicOutput.*)",
        R"(.*smoke_Auto_BehaviorTests.*DynamicOutputToDynamicInput.*)",
        R"(.*smoke_Auto_BehaviorTests.*DynamicInputToDynamicOutput.*)",
        // unsupported metrics
        R"(.*OVGetMetricPropsTest.*OVGetMetricPropsTest.*(DEVICE_UUID|FULL_DEVICE_NAME_with_DEVICE_ID|DEVICE_GOPS|DEVICE_TYPE|MAX_BATCH_SIZE).*)",
        R"(.*smoke_AutoMultiHeteroOVGetMetricPropsTest.*OVGetMetricPropsTest.*(AVAILABLE_DEVICES|OPTIMIZATION_CAPABILITIES|RANGE_FOR_ASYNC_INFER_REQUESTS|RANGE_FOR_STREAMS).*)",
        // supports only '' as device id
        R"(.*OVClassQueryModelTest.*QueryModelWithDeviceID.*)",

        // Issue 67214
        R"(smoke_PrePostProcess.*resize_and_convert_layout_i8.*)",
        // TODO: CVS-67255
        R"(smoke_If.*SimpleIf2OutTest.*)",

        // Issue: 69086
        // need to add support convert BIN -> FP32
        // if we set output precision as BIN, when we create output blob precision looks like UNSPECIFIED
        R"(.*smoke_FakeQuantizeLayerCPUTest.*bin.*)",
        // Issue: 69088
        // bad accuracy
        R"(.*smoke_FakeQuantizeLayerCPUTest_Decompos.
            *IS=_TS=\(\(4\.5\.6\.7\)\)_RS=\(\(1\.1\.6\.1\)\)_\(\(1\.5\.6\.1\)\)_\(\(1\.1\.1\.1\)\)_\(\(1\.1\.6\.1\)\).*)",
        // Issue: 69222
        R"(.*smoke_PriorBoxClustered.*PriorBoxClusteredLayerCPUTest.*_netPRC=f16_.*)",
        // Issue: 72005
        // there are some inconsistency between cpu plugin and ng ref
        // for ctcMergeRepeated is true when legal randomized inputs value.
        // Failure happened on win and macos for current seeds.
        R"(.*CTCLossLayerTest.*CMR=1.*)",
        R"(.*CTCLossLayerCPUTest.*ctcMergeRepeated=1.*)",
        // Issue: 71756
        R"(.*GroupDeconv_2D_DW_BF16/GroupDeconvolutionLayerCPUTest.CompareWithRefs.*PRC=f32.*inFmts=nChw16c_outFmts=nChw16c_primitive=jit_avx512_dw_Fused=Multiply\(PerChannel\).Add\(PerChannel\)_PluginConf_ENFORCE_BF16=YES.*)",
        R"(.*smoke_GroupDeconv_(2|3)D_Blocked_BF16.*S=(\(2\.2\)|\(2\.2\.2\))_PB=(\(0\.0\)|\(0\.0\.0\))_PE=(\(0\.0\)|\(0\.0\.0\))_D=(\(1\.1\)|\(1\.1\.1\))_.*_O=64_G=4.*)",
        // Issue: 59594
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*BOOL.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*MIXED.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*Q78.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*U4.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*I4.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*BIN.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*CUSTOM.*)",
        R"(smoke_ConversionLayerTest/ConversionLayerTest.CompareWithRefs.*UNSPECIFIED.*)",
        // Issue:
        // New API tensor tests
        R"(.*OVInferRequestCheckTensorPrecision.*type=i4.*)",
        R"(.*OVInferRequestCheckTensorPrecision.*type=u1.*)",
        R"(.*OVInferRequestCheckTensorPrecision.*type=u4.*)",
        // Issue: 77390
        R"(.*LoopLayerCPUTest.*exec_cond=0.*)",
        R"(.*LoopLayerCPUTest.*trip_count=0.*)",
        R"(.*LoopForDiffShapesLayerCPUTest.*exec_cond=0.*)",
        R"(.*LoopForDiffShapesLayerCPUTest.*trip_count=0.*)",
        R"(.*LoopForConcatLayerCPUTest.*exec_cond=0.*)",
        R"(.*LoopForConcatLayerCPUTest.*trip_count=0.*)",
        // [ INFO ] Can't compile network without cache for ..  with precision ..
        R"(.*CompileModelCacheTestBase.*CompareWithRefImpl.*KSOFunction.*)",
        R"(.*CompileModelCacheTestBase.*CompareWithRefImpl.*NonMaxSuppression.*)",
        R"(.*CompileModelCacheTestBase.*CompareWithRefImpl.*Nms.*)",
        // Issue: 76980
        R"(.*smoke_Auto_BehaviorTests.*InferDynamicNetwork/.*)",
        // Issue: 105838
        R"(smoke_NmsLayerTest.*)",
        // Issue: 95590
        R"(.*CachingSupportCase.*CompileModelCacheTestBase.*(TIwithLSTMcell1|MatMulBias|2InputSubtract)_(u|i).*)",
        // Issue: 95607
        R"(.*CachingSupportCase.*LoadNetworkCacheTestBase.*(TIwithLSTMcell1|MatMulBias|2InputSubtract)_(i|u).*)",
        R"(.*CachingSupportCase.*ReadConcatSplitAssign.*)",
        // 94982. FP32->I32 conversion issue in the reference implementation. There can be some garbage in the rest of float values like 0.333333745.
        // The kernel does not have such garbage. The diff 0.000000745 is taken into account in calculations and affects further type conversion.
        // Reorder->GridSample->Reorder also does not work here. Potential fix is to use nearest conversion instead of truncation.
        R"(.*GridSampleLayerTestCPU.*(BILINEAR|BICUBIC).*(i32|i8).*)",
        // // Issue: 95915
        R"(smoke_dynamic/AUGRUCellCPUTest.CompareWithRefs/IS=\(\[\?\.1\]_\[\?\.1\]_\[\?\.1\]_\)_TS=\{\(1\.1\)_\(1\.1\)_\(1\.1\)\}_\{\(3\.1\)_\(3\.1\)_\(3\.1\)\}_\{\(5\.1\)_\(5\.1\)_\(5\.1\)\}_decompose=0_activations=\(sigmoid\.tanh\)_clip=0_linear=0_netPrec=f32__inFmts=nc\.nc_outFmts=nc_primitive=ref_any_PluginConf_ENFORCE_BF16=YES)", // NOLINT
        R"(smoke_dynamic/GRUCellCPUTest.CompareWithRefs/IS=\(\[\?.1\]_\[\?\.1\]_\)_TS=\{\(1\.1\)_\(1\.1\)\}_\{\(3\.1\)_\(3\.1\)\}_\{\(5\.1\)_\(5\.1\)\}_decompose=0_activations=\(sigmoid\.tanh\)_clip=0_linear=0_netPrec=f32__inFmts=nc\.nc_outFmts=nc_primitive=ref_any_PluginConf_ENFORCE_BF16=YES)", // NOLINT
        R"(nightly_dynamic_bf16/RNNSequenceCPUTest.*activations=\(relu\).*)",
        R"(smoke_dynamic_BatchSizeOne/RNNSequenceCPUTest.*IS=\(\[1\.\?\.10\]_\[1\.1\.10\]_\[\?\]_\)_TS=\{\(1\.2\.10\)_\(1\.1\.10\)_\(1\)\}_\{\(1\.4\.10\)_\(1\.1\.10\)_\(1\)\}_\{\(1\.8\.10\)_\(1\.1\.10\)_\(1\)\}_seqMode=PURE_SEQ_activations=\(relu\)_clip=0_direction=forward_netPrec=f32__inFmts=ncw\.ntc_outFmts=ncw\.ncw_primitive=ref_any)", // NOLINT
        // 98151. Not valid sorting for slices in reference.
        R"(.*UniqueLayerTestCPU.*axis.*True.*)",
        // Issue: 104402. Incorrect broadcasting in FQ reference implentation
        R"(.*smoke_FakeQuantizeLayerCPUTest_Decompos.*IS=\[4\.5\.6\.6\]_TS=\(\(4\.5\.6\.6\)\)_RS=\(\(1\.1\.6\.6\)\)_\(\(1\.1\.6\.6\)\)_\(\(1\.5\.6\.1\)\)_\(\(1\.5\.1\.6\)\).*)",
        R"(.*smoke_FakeQuantizeLayerCPUTest_Decompos.*IS=\[4\.5\.6\.6\]_TS=\(\(4\.5\.6\.6\)\)_RS=\(\(1\.5\.6\.1\)\)_\(\(1\.5\.6\.1\)\)_\(\(1\.5\.6\.1\)\)_\(\(1\.5\.1\.6\)\).*)",
    };

#define FIX_62820 0
#if FIX_62820 && ((IE_THREAD == IE_THREAD_TBB) || (IE_THREAD == IE_THREAD_TBB_AUTO))
    retVector.emplace_back(R"(.*ReusableCPUStreamsExecutor.*)");
#endif

#ifdef __APPLE__
    // TODO: Issue 55717
    // retVector.emplace_back(R"(.*smoke_LPT.*ReduceMinTransformation.*f32.*)");
#endif

#if defined(OPENVINO_ARCH_ARM64) || defined(OPENVINO_ARCH_ARM)
    retVector.emplace_back(R"(OVClassBasicTest.smoke_SetConfigAffinity.*)");
    retVector.emplace_back(R"(OVClassConfigTestCPU.smoke_Check(Model|Core)StreamsHasHigherPriorityThanLatencyHint.*)");
    retVector.emplace_back(R"(.*OVInferRequestDynamicTests.*)");
    retVector.emplace_back(R"(ONNXQuantizedModels/QuantizedModelsTests.*)");
    retVector.emplace_back(R"(smoke_serialization/ExecGraphSerializationTest.ExecutionGraph.*)");
    retVector.emplace_back(R"(smoke_BehaviorTests/OVCompiledModelPropertiesDefaultTests.CheckDefaultValues.*)");
    retVector.emplace_back(R"(smoke_BehaviorTests/CorrectConfigCheck.canSetConfigAndCheckGetConfig.*CPU_THROUGHPUT_STREAMS=8.*)");
    retVector.emplace_back(R"(smoke_BehaviorTests/CorrectConfigCheck.canSetConfigTwiceAndCheckGetConfig.*CPU_THROUGHPUT_STREAMS=8.*)");
    retVector.emplace_back(R"(smoke_Basic/DefaultConfigurationTest.checkDeviceDefaultConfigurationValue/configKey=CPU_BIND_THREAD.*)");
    retVector.emplace_back(R"(smoke_ExecGraph/ExecGraphRuntimePrecision.CheckRuntimePrecision/Function=(EltwiseWithTwoDynamicInputs|FakeQuantizeRelu).*)");
    retVector.emplace_back(R"(smoke_LPT.*)");
    retVector.emplace_back(R"(smoke_CPU_OVClassLoadNetworkAndCheckWithSecondaryPropertiesTest/OVClassLoadNetworkAndCheckSecondaryPropertiesTest.LoadNetworkAndCheckSecondaryPropertiesTest.*)");
    retVector.emplace_back(R"(smoke_CPU_OVClassLoadNetworkAndCheckWithSecondaryPropertiesDoubleTest/OVClassLoadNetworkAndCheckSecondaryPropertiesTest.LoadNetworkAndCheckSecondaryPropertiesTest.*)");
    retVector.emplace_back(R"(smoke_CPU_OVClassCompileModelAndCheckSecondaryPropertiesTest/OVClassCompileModelAndCheckSecondaryPropertiesTest.CompileModelAndCheckSecondaryPropertiesTest.*)");
    retVector.emplace_back(R"(smoke_CPU_OVClassCompileModelAndCheckSecondaryPropertiesDoubleTest/OVClassCompileModelAndCheckSecondaryPropertiesTest.CompileModelAndCheckSecondaryPropertiesTest.*)");
    retVector.emplace_back(R"(smoke_Activation_Basic/ActivationLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_Integer_Activation_Basic/ActivationLayerTest.CompareWithRefs/(Tanh|Negative|Sqrt).*)");
    retVector.emplace_back(R"(smoke_Activation_Basic_Prelu_Const/ActivationLayerTest.CompareWithRefs/(LeakyRelu|PReLu).*)");
    retVector.emplace_back(R"(smoke_Activation_Basic_Prelu_Param/ActivationParamLayerTest.CompareWithRefs/(LeakyRelu|PReLu).*)");
    retVector.emplace_back(R"(smoke_CompareWithRefs/ComparisonLayerTest.ComparisonTests.*)");
    retVector.emplace_back(R"(smoke_CompareWithRefs_static/EltwiseLayerTest.EltwiseTests.*)");
    retVector.emplace_back(R"(smoke_CompareWithRefs_static_check_collapsing/EltwiseLayerTest.EltwiseTests.*)");
    retVector.emplace_back(R"(smoke_SingleThread/EltwiseLayerTest.EltwiseTests.*)");
    retVector.emplace_back(R"(smoke_GRUSequenceCommonZeroClip/GRUSequenceTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_GRUSequenceCommonClip/GRUSequenceTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_StaticShapeLoop/StaticShapeLoopTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_StaticShapeLoop/StaticShapeLoopTest.CompareWithPredefinedRefs.*)");
    retVector.emplace_back(R"(smoke_TrivialLoop/TrivialLoopTest.AutoSlicingInputWithDynCondition_(CheckPredefinedValues|CheckReference).*)");
    retVector.emplace_back(R"(smoke_LSTMSequenceCommonZeroClip/LSTMSequenceTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_LSTMSequenceCommonClip/LSTMSequenceTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_Decomposition_(3|4)D/Mvn6LayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_AvgPool_ExplicitPad_CeilRounding/PoolingLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_ReduceLogicalOneAxis/ReduceOpsLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_Reduce_InputShapes/ReduceOpsLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_ReduceLogical_ReductionTypes/ReduceOpsLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_RNNSequenceCommonZeroClip/RNNSequenceTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_RNNSequenceCommonClip/RNNSequenceTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_INTEL_CPU_TestsDFT_(1|2|3|4)d/DFTLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_INTEL_CPU_TestsSelect_none/SelectLayerTest.CompareWithRefImpl/COND=BOOL.*)");
    retVector.emplace_back(R"(smoke_INTEL_CPU_TestsSelect_numpy/SelectLayerTest.CompareWithRefImpl/COND=BOOL.*)");
    retVector.emplace_back(R"(smoke_Snippets.*)");
    retVector.emplace_back(R"(smoke_Quantized.*)");
    retVector.emplace_back(R"(smoke_NegativeQuantizedMatMulMultiplyFusion.*)");
    retVector.emplace_back(R"(MultipleLSTMCellTest/MultipleLSTMCellTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_MultipleAdd_Nd/MultiplyAddLayerTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_If/SimpleIfTest.CompareWithRefs.*)");
    retVector.emplace_back(R"(smoke_If/SimpleIfNotConstConditionTest.CompareWithRefs.*)");
#endif

#if defined(_WIN32) || defined(_WIN64)
    retVector.emplace_back(R"(.*LoadNetworkCompiledKernelsCacheTest.*CanCreateCacheDirAndDumpBinariesUnicodePath.*)");
#endif

    if (!InferenceEngine::with_cpu_x86_avx512_core()) {
        // on platforms which do not support bfloat16, we are disabling bf16 tests since there are no bf16 primitives,
        // tests are useless on such platforms
        retVector.emplace_back(R"(.*(BF|bf)16.*)");
        retVector.emplace_back(R"(.*bfloat16.*)");
        // MatMul in Snippets uses BRGEMM that is supported only on AVX512 platforms
        // Disabled Snippets MHA tests as well because MHA pattern contains MatMul
        retVector.emplace_back(R"(.*Snippets.*MHA.*)");
        retVector.emplace_back(R"(.*Snippets.*(MatMul|Matmul).*)");
    }
    if (!InferenceEngine::with_cpu_x86_avx512_core_vnni() && !InferenceEngine::with_cpu_x86_avx512_core_amx_int8()) {
        // MatMul in Snippets uses BRGEMM that supports i8 only on platforms with VNNI or AMX instructions
        retVector.emplace_back(R"(.*Snippets.*MatMulFQ.*)");
    }
    if (!InferenceEngine::with_cpu_x86_avx512_core_amx_int8())
        //TODO: Issue 92895
        // on platforms which do not support AMX, we are disabling I8 input tests
        retVector.emplace_back(R"(smoke_LPT/FakeQuantizeWithNotOptimalTransformation.CompareWithRefImpl.*CPU.*I8.*)");
    if (!InferenceEngine::with_cpu_x86_avx512_core_amx_bf16() && !InferenceEngine::with_cpu_x86_bfloat16()) {
        // ignored for not supported bf16 platforms
        retVector.emplace_back(R"(.*smoke_Snippets_EnforcePrecision_bf16.*)");
    }

    return retVector;
}
