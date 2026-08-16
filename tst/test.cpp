#include "mkn/mod/init.hpp"

#include <iostream>

namespace {

class FakeContext : public mkn::mod::Context, public mkn::mod::CompilerState {
 public:
  mkn::mod::ContextState state() const override {
    mkn::mod::ContextState s;
    s.projectDir = dir;
    s.includes = incs;
    s.dependents = rdeps;
    s.buildMode = m;
    return s;
  }
  mkn::mod::CompilerState& compilerState() override { return *this; }
  void per_compiler_command(CompileHook) override {}
  std::string compileCommandFor(std::string const&) const override { return ""; }

  void add(mkn::mod::AbstractCompilerInput const& input) override {
    if (auto const* i = dynamic_cast<mkn::mod::IncludeInput const*>(&input))
      incs.emplace_back(i->path, i->is_public);
    else if (auto const* i = dynamic_cast<mkn::mod::BuildModeInput const*>(&input))
      m = i->mode;
  }

  std::vector<std::pair<std::string, bool>> const& includes() const { return incs; }

 private:
  std::string dir{"."};
  std::vector<std::pair<std::string, bool>> incs;
  std::vector<mkn::mod::Context*> rdeps;
  mkn::mod::Mode m = mkn::mod::Mode::SHAR;
};

class UsageModule : public mkn::mod::Module {
 public:
  void compile(mkn::mod::Context& c, YAML::Node const&) override {
    c.compilerState().add(mkn::mod::IncludeInput{"./inc", true});
  }
};

}  // namespace

extern "C" MKN_KUL_PUBLISH mkn::mod::Module* maiken_module_construct() { return new UsageModule; }
extern "C" MKN_KUL_PUBLISH void maiken_module_destruct(mkn::mod::Module* p) { delete p; }

int main() {
  FakeContext ctx;
  UsageModule mod;
  YAML::Node node;
  mod.compile(ctx, node);
  std::cout << "mkn.mod usage OK, includes=" << ctx.includes().size() << std::endl;
  return 0;
}
