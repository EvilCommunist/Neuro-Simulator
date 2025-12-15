#ifndef GENETICOPERATORSENUM_H
#define GENETICOPERATORSENUM_H

#define WEAK_MUTATION 0.25
#define NORMAL_MUTATION 0.5

enum Selection{
    TOURNAMENT,
    PROPORTIONAL,
    RANGED
};

enum Crossover{
    ARITHMETICAL,
    BLX_ALPHA,
    LINEAR,
    ONE_POINT,
    TWO_POINT,
    EVEN
};

enum MutationStrength{
    WEAK,   // 25% that each weight will mutate
    NORMAL, // 50% that each weight will mutate
    STRONG  // 100% that each weight will mutate
};

#endif // GENETICOPERATORSENUM_H
