// $F_0 = 0, \quad F_1 = 1, \quad F_n = F_{n-1} + F_{n-2}$

// $F_n = \frac{\left(\frac{1+\sqrt5}2\right)^n-\left(\frac{1-\sqrt5}2\right)^n}{\sqrt5}$

// $\begin{bmatrix}1 & 1 \\ 1 & 0\end{bmatrix}^n=\begin{bmatrix}
//     F_{n+1} & F_{n} \\
//     F_n & F_{n-1}
// \end{bmatrix}$

// $F(n+1)=\sum\limits_{k=0}^{\lfloor n/2\rfloor}\binom{n-k}{k}$


$F _(0 ) =  0 \, quad  F _(1 ) =  1 \, quad  F _(n ) =  F _(n - 1 ) +  F _(n - 2 )$

$F _(n ) =  frac(lr(\( frac(1 + sqrt(5 ),2 )\) )^(n )- lr(\( frac(1 - sqrt(5 ),2 )\) )^(n ),sqrt(5 ))$

$mat(1  zws , 1  zws ; 1  zws , 0 )^(n )= mat(
F _(n + 1 ) zws , F _(n ) zws ;
F _(n ) zws , F _(n - 1 )
)$

$F \(n + 1 \)= limits(sum )_(k = 0 )^(⌊  n \/2 ⌋ )binom(n - k ,k )
$