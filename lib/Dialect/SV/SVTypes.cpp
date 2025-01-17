//===- SVTypes.cpp - Implement the SV types -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implement the SV dialect type system.
//
//===----------------------------------------------------------------------===//

#include "circt/Dialect/SV/SVTypes.h"
#include "circt/Dialect/RTL/RTLTypes.h"
#include "circt/Dialect/SV/SVDialect.h"

#include "mlir/IR/DialectImplementation.h"
#include "llvm/ADT/TypeSwitch.h"

using namespace circt;
using namespace circt::sv;

/// Return the element type of an ArrayType or UnpackedArrayType, or null if the
/// operand isn't an array.
Type circt::sv::getAnyRTLArrayElementType(Type type) {
  if (!type)
    return {};
  if (auto array = type.dyn_cast<rtl::ArrayType>())
    return array.getElementType();
  if (auto array = type.dyn_cast<rtl::UnpackedArrayType>())
    return array.getElementType();
  return {};
}

//===----------------------------------------------------------------------===//
// InOut type logic.
//===----------------------------------------------------------------------===//

/// Return the element type of an InOutType or null if the operand isn't an
/// InOut type.
mlir::Type circt::sv::getInOutElementType(mlir::Type type) {
  if (auto inout = type.dyn_cast_or_null<InOutType>())
    return inout.getElementType();
  return {};
}

//===----------------------------------------------------------------------===//
// SV Interface type logic.
//===----------------------------------------------------------------------===//

Type InterfaceType::parse(MLIRContext *ctxt, DialectAsmParser &p) {
  FlatSymbolRefAttr iface;
  if (p.parseLess() || p.parseAttribute(iface) || p.parseGreater())
    return Type();
  return get(ctxt, iface);
}

void InterfaceType::print(DialectAsmPrinter &p) const {
  p << "interface<" << getInterface() << ">";
}

Type ModportType::parse(MLIRContext *ctxt, DialectAsmParser &p) {
  SymbolRefAttr modPort;
  if (p.parseLess() || p.parseAttribute(modPort) || p.parseGreater())
    return Type();
  return get(ctxt, modPort);
}

void ModportType::print(DialectAsmPrinter &p) const {
  p << "modport<" << getModport() << ">";
}

//===----------------------------------------------------------------------===//
// TableGen generated logic.
//===----------------------------------------------------------------------===//

// Provide the autogenerated implementation guts for the Op classes.
#define GET_TYPEDEF_CLASSES
#include "circt/Dialect/SV/SVTypes.cpp.inc"

/// Parses a type registered to this dialect
Type SVDialect::parseType(DialectAsmParser &parser) const {
  llvm::StringRef mnemonic;
  auto loc = parser.getCurrentLocation();
  if (parser.parseKeyword(&mnemonic))
    return Type();
  Type type;
  auto parseResult = generatedTypeParser(getContext(), parser, mnemonic, type);
  if (parseResult.hasValue())
    return type;
  parser.emitError(loc, "Failed to parse type sv.") << mnemonic << "\n";
  return Type();
}

/// Print a type registered to this dialect
void SVDialect::printType(Type type, DialectAsmPrinter &printer) const {
  if (succeeded(generatedTypePrinter(type, printer)))
    return;
  llvm_unreachable("unexpected 'rtl' type kind");
}

void SVDialect::registerTypes() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "circt/Dialect/SV/SVTypes.cpp.inc"
      >();
}