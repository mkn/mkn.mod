# mkn.mod

**Interface for [maiken](https://github.com/mkn/mkn) modules**

Header-only. A module is a shared library maiken loads during a build, handed a `mkn::mod::Context`
for the project it is used in, to add include paths, libraries, compile arguments and more.

## Example

A module that adds `./inc` as a public include path, see [tst/mod/mod.cpp](tst/mod/mod.cpp):

```cpp
#include "mkn/mod/init.hpp"

namespace {

class UsageModule : public mkn::mod::Module {
 public:
  void compile(mkn::mod::Context& c, YAML::Node const&) override {
    c.compilerState().add(mkn::mod::IncludeInput{"./inc", true});
  }
};

}  // namespace

extern "C" MKN_KUL_PUBLISH mkn::mod::Module* maiken_module_construct() { return new UsageModule; }
extern "C" MKN_KUL_PUBLISH void maiken_module_destruct(mkn::mod::Module* p) { delete p; }
```

`mkn/mod/init.hpp` must be included before any other `mkn.kul` header, so the exported symbols get
the right visibility.

[inc/mkn/mod/loader.hpp](inc/mkn/mod/loader.hpp) loads a module from its shared library, as used
in [tst/test.cpp](tst/test.cpp).

## Usage

In a module's `mkn.yaml`:

```yaml
dep: mkn.mod
mode: shared
```

Build and run the tests:

```bash
mkn clean build test run -p test_mod,test -W 9
```

## Dependencies

[mkn.kul](https://github.com/mkn/mkn.kul), [yaml-cpp](https://github.com/jbeder/yaml-cpp)
