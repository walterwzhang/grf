# grf: generalized random forests

A pluggable package for forest-based statistical estimation and inference. GRF currently provides non-parametric methods for least-squares regression, quantile regression, and treatment effect estimation (optionally using instrumental variables).

In addition, GRF supports 'honest' estimation (where one subset of the data is used for choosing splits, and another for populating the leaves of the tree), and confidence intervals for least-squares regression and treatment effect estimation.

This package is currently in beta, and we expect to make continual improvements to its performance and usability.

### Authors

The original package is written and maintained by Julie Tibshirani (jtibs@cs.stanford.edu), Susan Athey, and Stefan Wager and can be found [here](https://github.com/swager/grf).

This fork of the grf package is used for didatic purposes and will not be up to date with the latest version.

### Installation

To install the package run the following command:

```R
install.packages("https://raw.github.com/walterwzhang/grf/master/releases/grf_0.9.5.tar.gz",
                 repos = NULL, type = "source")
```

Note that to install from source, a compiler that implements C++11 is required (clang 3.3 or higher, or g++ 4.8 or higher). If installing on Windows, the RTools toolchain is also required.

### Usage Examples

```R
library(grf)

# Generate data.
n   <-   2000
p   <-   10
X   <-   matrix(rnorm(n*p), n, p)
X.test       <-   matrix(0, 101, p)
X.test[,1]   <-   seq(-2, 2, length.out = 101)


# Perform treatment effect estimation
W   <-   rbinom(n, 1, 0.5)
Y   <-   pmax(X[,1], 0) * W + X[,2] + pmin(X[,3], 0) + rnorm(n)

tau.forest   <-   causal_forest(X, Y, W, num.trees = 1000)
tau.hat      <-   predict(tau.forest, X.test)
plot(x = X.test[,1],
     y = tau.hat$predictions,
     ylim = range(tau.hat$predictions, 0, 2),
     xlab = "x", ylab = "tau", type = "l")
lines(X.test[,1], pmax(0, X.test[,1]), col = 2, lty = 2)


# Estimate the conditional average treatment effect on the full sample (CATE)
estimate_average_effect(tau.forest, target.sample = "all")


# Estimate the conditional average treatment effect on the treated sample (CATT)
# In this simulation, we don't expect much difference between the CATE
# and the CATT, since treatment assignment was randomized.
estimate_average_effect(tau.forest, target.sample = "treated")


# Add confidence intervals for heterogeneous treatment effects
## Growing more trees is now recommended
tau.forest   <-   causal_forest(X, Y, W, num.trees = 4000)
tau.hat      <-   predict(tau.forest, X.test, estimate.variance = TRUE)
sigma.hat    <-    sqrt(tau.hat$variance.estimates)
plot(x = X.test[,1],
     y = tau.hat$predictions,
     ylim = range(tau.hat$predictions + 1.96 * sigma.hat,
                  tau.hat$predictions - 1.96 * sigma.hat, 0, 2),
     xlab = "x", ylab = "tau", type = "l")
lines(X.test[,1], tau.hat$predictions + 1.96 * sigma.hat, col = 1, lty = 2)
lines(X.test[,1], tau.hat$predictions - 1.96 * sigma.hat, col = 1, lty = 2)
lines(X.test[,1], pmax(0, X.test[,1]), col = 2, lty = 1)
```

For examples on how to use other types of forests, including those for [quantile regression](https://github.com/swager/grf/blob/master/documentation/quantile_examples.md) and causal effect estimation using instrumental variables, please see the `documentation` directory in the original repository.

### References

Susan Athey, Julie Tibshirani and Stefan Wager.
<b>Generalized Random Forests</b>, 2016.
[<a href="https://arxiv.org/abs/1610.01271">arxiv</a>]
