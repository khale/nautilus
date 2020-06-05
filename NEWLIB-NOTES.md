# Newlib porting notes

* we need a cross-compiler. libgcc is making too many linux assumptions, and cc flags are getting messy
* __builtin_popcount is causing problems. We have an impl in libcompat, but that's going to get blown away. The presence
of this instruction is arch-dependent. 
