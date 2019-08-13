#include "lib/effect_runner.h"
#include "lib/brightness.h"
#include "patterns.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    EffectRunner r;
    PatternsEffect e;
    Brightness b(e);

    //b.setAssumedGamma(2.2);
    b.set(0.1); 

    r.setEffect(&b);
    r.setLayout("./layouts/multi_strip_112.json");

    return r.main(argc, argv);
}
