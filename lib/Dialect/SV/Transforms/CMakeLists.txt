add_circt_dialect_library(CIRCTSVTransforms
  RTLCleanup.cpp
  RTLStubExternalModules.cpp
  RTLLegalizeNames.cpp

  DEPENDS
  CIRCTSVTransformsIncGen

  LINK_LIBS PUBLIC
  CIRCTSV
  CIRCTSVAnalyses
  MLIRIR
  MLIRPass
  MLIRTransformUtils
)
