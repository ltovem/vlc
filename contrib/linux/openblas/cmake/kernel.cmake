# helper functions for the kernel CMakeLists.txt


# Set the default filenames for L1 objects. Most of these will be overridden by the appropriate KERNEL file.
macro(SetDefaultL1)
  set(SAMAXKERNEL amax.S)
  set(DAMAXKERNEL amax.S)
  set(QAMAXKERNEL amax.S)
  set(CAMAXKERNEL zamax.S)
  set(ZAMAXKERNEL zamax.S)
  set(XAMAXKERNEL zamax.S)
  set(SAMINKERNEL amin.S)
  set(DAMINKERNEL amin.S)
  set(QAMINKERNEL amin.S)
  set(CAMINKERNEL zamin.S)
  set(ZAMINKERNEL zamin.S)
  set(XAMINKERNEL zamin.S)
  set(SMAXKERNEL max.S)
  set(DMAXKERNEL max.S)
  set(QMAXKERNEL max.S)
  set(SMINKERNEL min.S)
  set(DMINKERNEL min.S)
  set(QMINKERNEL min.S)
  set(ISAMAXKERNEL iamax.S)
  set(IDAMAXKERNEL iamax.S)
  set(IQAMAXKERNEL iamax.S)
  set(ICAMAXKERNEL izamax.S)
  set(IZAMAXKERNEL izamax.S)
  set(IXAMAXKERNEL izamax.S)
  set(ISAMINKERNEL iamin.S)
  set(IDAMINKERNEL iamin.S)
  set(IQAMINKERNEL iamin.S)
  set(ICAMINKERNEL izamin.S)
  set(IZAMINKERNEL izamin.S)
  set(IXAMINKERNEL izamin.S)
  set(ISMAXKERNEL iamax.S)
  set(IDMAXKERNEL iamax.S)
  set(IQMAXKERNEL iamax.S)
  set(ISMINKERNEL iamin.S)
  set(IDMINKERNEL iamin.S)
  set(IQMINKERNEL iamin.S)
  set(SASUMKERNEL asum.S)
  set(DASUMKERNEL asum.S)
  set(CASUMKERNEL zasum.S)
  set(ZASUMKERNEL zasum.S)
  set(QASUMKERNEL asum.S)
  set(XASUMKERNEL zasum.S)
  set(SAXPYKERNEL axpy.S)
  set(DAXPYKERNEL axpy.S)
  set(CAXPYKERNEL zaxpy.S)
  set(ZAXPYKERNEL zaxpy.S)
  set(QAXPYKERNEL axpy.S)
  set(XAXPYKERNEL zaxpy.S)
  set(SCOPYKERNEL copy.S)
  set(DCOPYKERNEL copy.S)
  set(CCOPYKERNEL zcopy.S)
  set(ZCOPYKERNEL zcopy.S)
  set(QCOPYKERNEL copy.S)
  set(XCOPYKERNEL zcopy.S)
  set(SDOTKERNEL dot.S)
  set(DDOTKERNEL dot.S)
  set(CDOTKERNEL zdot.S)
  set(ZDOTKERNEL zdot.S)
  set(QDOTKERNEL dot.S)
  set(XDOTKERNEL zdot.S)
  set(SNRM2KERNEL nrm2.S)
  set(DNRM2KERNEL nrm2.S)
  set(QNRM2KERNEL nrm2.S)
  set(CNRM2KERNEL znrm2.S)
  set(ZNRM2KERNEL znrm2.S)
  set(XNRM2KERNEL znrm2.S)
  set(SROTKERNEL rot.S)
  set(DROTKERNEL rot.S)
  set(QROTKERNEL rot.S)
  set(CROTKERNEL zrot.S)
  set(ZROTKERNEL zrot.S)
  set(XROTKERNEL zrot.S)
  set(SSCALKERNEL scal.S)
  set(DSCALKERNEL scal.S)
  set(CSCALKERNEL zscal.S)
  set(ZSCALKERNEL zscal.S)
  set(QSCALKERNEL scal.S)
  set(XSCALKERNEL zscal.S)
  set(SSWAPKERNEL swap.S)
  set(DSWAPKERNEL swap.S)
  set(CSWAPKERNEL zswap.S)
  set(ZSWAPKERNEL zswap.S)
  set(QSWAPKERNEL swap.S)
  set(XSWAPKERNEL zswap.S)
  set(SGEMVNKERNEL gemv_n.S)
  set(SGEMVTKERNEL gemv_t.S)
  set(DGEMVNKERNEL gemv_n.S)
  set(DGEMVTKERNEL gemv_t.S)
  set(CGEMVNKERNEL zgemv_n.S)
  set(CGEMVTKERNEL zgemv_t.S)
  set(ZGEMVNKERNEL zgemv_n.S)
  set(ZGEMVTKERNEL zgemv_t.S)
  set(QGEMVNKERNEL gemv_n.S)
  set(QGEMVTKERNEL gemv_t.S)
  set(XGEMVNKERNEL zgemv_n.S)
  set(XGEMVTKERNEL zgemv_t.S)
  set(SCABS_KERNEL ../generic/cabs.c)
  set(DCABS_KERNEL ../generic/cabs.c)
  set(QCABS_KERNEL ../generic/cabs.c)
  set(LSAME_KERNEL ../generic/lsame.c)
  set(SAXPBYKERNEL ../arm/axpby.c)
  set(DAXPBYKERNEL ../arm/axpby.c)
  set(CAXPBYKERNEL ../arm/zaxpby.c)
  set(ZAXPBYKERNEL ../arm/zaxpby.c)
  set(SSUMKERNEL sum.S)
  set(DSUMKERNEL sum.S)
  set(CSUMKERNEL zsum.S)
  set(ZSUMKERNEL zsum.S)
  set(QSUMKERNEL sum.S)
  set(XSUMKERNEL zsum.S)
if (BUILD_BFLOAT16)
  set(SHAMINKERNEL ../arm/amin.c)
  set(SHAMAXKERNEL ../arm/amax.c)
  set(SHMAXKERNEL ../arm/max.c)
  set(SHMINKERNEL ../arm/min.c)
  set(ISHAMAXKERNEL ../arm/iamax.c)
  set(ISHAMINKERNEL ../arm/iamin.c)
  set(ISHMAXKERNEL ../arm/imax.c)
  set(ISHMINKERNEL ../arm/imin.c)
  set(SHASUMKERNEL ../arm/asum.c)
  set(SHAXPYKERNEL ../arm/axpy.c)
  set(SHAXPBYKERNEL ../arm/axpby.c)
  set(SHCOPYKERNEL ../arm/copy.c)
  set(SBDOTKERNEL ../x86_64/sbdot.c)
  set(SHROTKERNEL ../arm/rot.c)
  set(SHSCALKERNEL ../arm/scal.c)
  set(SHNRM2KERNEL ../arm/nrm2.c)
  set(SHSUMKERNEL ../arm/sum.c)
  set(SHSWAPKERNEL ../arm/swap.c)
  set(TOBF16KERNEL ../x86_64/tobf16.c)
  set(BF16TOKERNEL ../x86_64/bf16to.c)
endif ()
endmacro ()

macro(SetDefaultL2)
  set(SGEMVNKERNEL ../arm/gemv_n.c)
  set(SGEMVTKERNEL ../arm/gemv_t.c)
  set(DGEMVNKERNEL gemv_n.S)
  set(DGEMVTKERNEL gemv_t.S)
  set(CGEMVNKERNEL zgemv_n.S)
  set(CGEMVTKERNEL zgemv_t.S)
  set(ZGEMVNKERNEL zgemv_n.S)
  set(ZGEMVTKERNEL zgemv_t.S)
  set(QGEMVNKERNEL gemv_n.S)
  set(QGEMVTKERNEL gemv_t.S)
  set(XGEMVNKERNEL zgemv_n.S)
  set(XGEMVTKERNEL zgemv_t.S)
  set(SGERKERNEL ../generic/ger.c)
  set(DGERKERNEL ../generic/ger.c)
  set(QGERKERNEL ../generic/ger.c)
  set(CGERUKERNEL ../generic/zger.c)
  set(CGERCKERNEL ../generic/zger.c)
  set(ZGERUKERNEL ../generic/zger.c)
  set(ZGERCKERNEL ../generic/zger.c)
  set(XGERUKERNEL ../generic/zger.c)
  set(XGERCKERNEL ../generic/zger.c)
  set(SSYMV_U_KERNEL ../generic/symv_k.c)
  set(SSYMV_L_KERNEL ../generic/symv_k.c)
  set(DSYMV_U_KERNEL ../generic/symv_k.c)
  set(DSYMV_L_KERNEL ../generic/symv_k.c)
  set(QSYMV_U_KERNEL ../generic/symv_k.c)
  set(QSYMV_L_KERNEL ../generic/symv_k.c)
  set(CSYMV_U_KERNEL ../generic/zsymv_k.c)
  set(CSYMV_L_KERNEL ../generic/zsymv_k.c)
  set(ZSYMV_U_KERNEL ../generic/zsymv_k.c)
  set(ZSYMV_L_KERNEL ../generic/zsymv_k.c)
  set(XSYMV_U_KERNEL ../generic/zsymv_k.c)
  set(XSYMV_L_KERNEL ../generic/zsymv_k.c)
  set(CHEMV_U_KERNEL ../generic/zhemv_k.c)
  set(CHEMV_L_KERNEL ../generic/zhemv_k.c)
  set(CHEMV_V_KERNEL ../generic/zhemv_k.c)
  set(CHEMV_M_KERNEL ../generic/zhemv_k.c)
  set(ZHEMV_U_KERNEL ../generic/zhemv_k.c)
  set(ZHEMV_L_KERNEL ../generic/zhemv_k.c)
  set(ZHEMV_V_KERNEL ../generic/zhemv_k.c)
  set(ZHEMV_M_KERNEL ../generic/zhemv_k.c)
  set(XHEMV_U_KERNEL ../generic/zhemv_k.c)
  set(XHEMV_L_KERNEL ../generic/zhemv_k.c)
  set(XHEMV_V_KERNEL ../generic/zhemv_k.c)
  set(XHEMV_M_KERNEL ../generic/zhemv_k.c)
if (BUILD_BFLOAT16)
  set(SBGEMVNKERNEL ../x86_64/sbgemv_n.c)
  set(SBGEMVTKERNEL ../x86_64/sbgemv_t.c)
  set(SHGERKERNEL ../generic/ger.c)
endif ()
endmacro ()

macro(SetDefaultL3)
  set(SGEADD_KERNEL ../generic/geadd.c)
  set(DGEADD_KERNEL ../generic/geadd.c)
  set(CGEADD_KERNEL ../generic/zgeadd.c)
  set(ZGEADD_KERNEL ../generic/zgeadd.c)
if (BUILD_BFLOAT16)
  set(SHGEADD_KERNEL ../generic/geadd.c)
  set(SBGEMMKERNEL ../generic/gemmkernel_2x2.c)
  set(SBGEMM_BETA  ../generic/gemm_beta.c)
  set(SBGEMMINCOPY ../generic/gemm_ncopy_2.c)
  set(SBGEMMITCOPY ../generic/gemm_tcopy_2.c)
  set(SBGEMMONCOPY ../generic/gemm_ncopy_2.c)
  set(SBGEMMOTCOPY ../generic/gemm_tcopy_2.c)
  set(SBGEMMINCOPYOBJ sbgemm_incopy.o)
  set(SBGEMMITCOPYOBJ sbgemm_itcopy.o)
  set(SBGEMMONCOPYOBJ sbgemm_oncopy.o)
  set(SBGEMMOTCOPYOBJ sbgemm_otcopy.o)
endif ()

endmacro ()
