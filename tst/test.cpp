#include "mkn/mod/init.hpp"
#include "mkn/mod/loader.hpp"

#include "mkn/kul/os.hpp"

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

mkn::kul::File find_test_module() {
  mkn::kul::Dir dir("bin/test_mod");
  for (auto const& f : dir.files(0)) {
    auto const& name = f.name();
    auto const dot = name.rfind(".");
    if (dot == std::string::npos) continue;
#if MKN_KUL_IS_WIN
    if (name.substr(dot + 1) == "dll") return mkn::kul::File(f.real());
#else
    if (name.substr(dot + 1) == "so") return mkn::kul::File(f.real());
#endif
  }
  KEXCEPTION("No loadable test module found in ", dir.escr());
}

}  // namespace

int main() {
  mkn::mod::Loader loader(find_test_module());
  FakeContext ctx;
  YAML::Node node;
  loader.module()->compile(ctx, node);
  std::cout << "mkn.mod usage OK, includes=" << ctx.includes().size() << std::endl;
  return 0;
}
