/*-------------------------------------------------------------------------------
  This file is part of generalized-random-forest.

  grf is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  grf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with grf. If not, see <http://www.gnu.org/licenses/>.
 #-------------------------------------------------------------------------------*/

#include "splitting/RegularizedRegressionSplittingRule.h"
#include "RegularizedRegressionSplittingRuleFactory.h"

RegularizedRegressionSplittingRuleFactory::RegularizedRegressionSplittingRuleFactory(double alpha,
                                                                                     bool downweight_penalty):
    lambda(alpha), downweight_penalty(downweight_penalty) {}

std::shared_ptr<SplittingRule> RegularizedRegressionSplittingRuleFactory::create(Data* data) {
  return std::shared_ptr<SplittingRule>(new RegularizedRegressionSplittingRule(data, lambda, downweight_penalty));
}

