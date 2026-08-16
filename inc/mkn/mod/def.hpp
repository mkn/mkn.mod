/**
Copyright (c) 2026, Philip Deegan.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Philip Deegan nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef MKN_MOD_DEF_HPP
#define MKN_MOD_DEF_HPP

#include "mkn/kul/lang/def.hpp"

#include "yaml-cpp/yaml.h"

namespace mkn::mod {

using Mode = mkn::kul::lang::Mode;
using CompilationInfo = mkn::kul::lang::CompilationInfo;
using CompileCommand = mkn::kul::lang::CompileCommand;
using CompileEnv = mkn::kul::lang::CompileEnv;
using Exception = mkn::kul::lang::Exception;

using ContextState = mkn::kul::lang::ContextState;
using CompilerState = mkn::kul::lang::CompilerState;
using AbstractCompilerInput = mkn::kul::lang::AbstractCompilerInput;
using IncludeInput = mkn::kul::lang::IncludeInput;
using LibPathInput = mkn::kul::lang::LibPathInput;
using LibInput = mkn::kul::lang::LibInput;
using LinkPrefixInput = mkn::kul::lang::LinkPrefixInput;
using BuildModeInput = mkn::kul::lang::BuildModeInput;
using CompilationInfoInput = mkn::kul::lang::CompilationInfoInput;

// Stable, versioned surface a maiken::Application implements and a Module is
// handed at each build phase. Promoted to mkn.kul as mkn::kul::lang::Context -
// this alias keeps existing module sources (mkn::mod::Context) unchanged.
using Context = mkn::kul::lang::Context;

class Module {
 public:
  virtual ~Module() = default;

  virtual void init(Context&, YAML::Node const&) {}
  virtual void compile(Context&, YAML::Node const&) {}
  virtual void link(Context&, YAML::Node const&) {}
  virtual void test(Context&, YAML::Node const&) {}
  virtual void pack(Context&, YAML::Node const&) {}
};

}  // namespace mkn::mod

#endif  // MKN_MOD_DEF_HPP
