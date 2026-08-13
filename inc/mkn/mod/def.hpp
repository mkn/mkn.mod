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

#include <string>
#include <utility>
#include <vector>

#include "yaml-cpp/yaml.h"

namespace mkn::mod {

enum class Mode { NONE = 0, STAT, SHAR };

// Stable, versioned surface a maiken::Application implements and a Module
// is handed at each build phase. Only append new virtuals here - never
// remove or reorder - so modules built against an older mkn.mod keep working
// unmodified against a newer maiken.
class Context {
 public:
  virtual ~Context() = default;

  virtual std::string projectDir() const = 0;
  virtual std::vector<std::pair<std::string, bool>> const& includes() const = 0;
  virtual void addInclude(std::string const& s, bool const is_public = true) = 0;
  virtual void addLibpath(std::string const& s) = 0;
  virtual void addLib(std::string const& s) = 0;
  virtual void prependLinkString(std::string const& s) = 0;
  // Named dependents()/buildMode() rather than revendencies()/mode() so a
  // maiken::Application (which already has same-named, differently-typed
  // members) can implement both without a return-type-only overload clash.
  virtual std::vector<Context*> dependents() const = 0;
  virtual Mode buildMode() const = 0;

  // Escape hatch for modules needing something not yet exposed above.
  // Returns the concrete maiken::Application*, so pulling in maiken/app.hpp
  // (and its ABI risk) is opt-in per-module rather than forced on everyone.
  virtual void* application() = 0;
};

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
