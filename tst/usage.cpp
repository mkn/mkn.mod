#include "mkn/mod/init.hpp"

#include <iostream>

namespace {

class FakeContext : public mkn::mod::Context {
 public:
  std::string projectDir() const override { return dir; }
  std::vector<std::pair<std::string, bool>> const& includes() const override { return incs; }
  void addInclude(std::string const& s, bool const is_public) override {
    incs.emplace_back(s, is_public);
  }
  void addLibpath(std::string const& s) override { paths.push_back(s); }
  void addLib(std::string const& s) override { libs.push_back(s); }
  void prependLinkString(std::string const& s) override { link = s + " " + link; }
  std::vector<mkn::mod::Context*> dependents() const override { return rdeps; }
  mkn::mod::Mode buildMode() const override { return m; }
  void* application() override { return nullptr; }

 private:
  std::string dir{"."};
  std::vector<std::pair<std::string, bool>> incs;
  std::vector<std::string> paths, libs;
  std::vector<mkn::mod::Context*> rdeps;
  std::string link;
  mkn::mod::Mode m = mkn::mod::Mode::SHAR;
};

class UsageModule : public mkn::mod::Module {
 public:
  void compile(mkn::mod::Context& c, YAML::Node const&) override { c.addInclude("./inc"); }
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
