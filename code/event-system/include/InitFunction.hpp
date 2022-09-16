#ifndef _INITFUNCTION_HPP
#define _INITFUNCTION_HPP

#define DEFINE_INIT_FUNCTION(n, f) static class IC_##n { public: IC_##n () { f } } gInit##n;

#endif // !_INITFUNCTION_HPP