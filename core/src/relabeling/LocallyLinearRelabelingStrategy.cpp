/*-------------------------------------------------------------------------------
 This file is part of gradient-forest...
 #-------------------------------------------------------------------------------*/

#include "utility.h"
#include "eigen3/Eigen/Dense"
#include "LocallyLinearRelabelingStrategy.h"

LocallyLinearRelabelingStrategy::LocallyLinearRelabelingStrategy(double lambda,
                                                                 const Data *data):
    lambda(lambda),
    data(data){}

// not sure about the syntax above here; must check

std::unordered_map<size_t, double> LocallyLinearRelabelingStrategy::relabel_outcomes(const Observations& observations,
                                                                                     const std::vector<size_t>& node_sampleIDs) {
    
    // find number of samples and covariates
    const size_t num_samples = node_sampleIDs.size();
    const size_t p = data->get_num_cols(); // check on usage
   
    // initialize theta
    Eigen::Matrix<double, p, 1> theta;
    
    // extract matrix X  and vector Y
    Eigen::Matrix<double, num_samples, p> X = Eigen::Matrix<double, num_samples, p>::Zero();
    Eigen::Matrix<double, num_samples, 1> Y = Eigen::Matrix<double, num_samples, 1>::Zero();
    
    int i = 0;
    for (size_t sampleID : node_sampleIDs) {
        for(size_t j; j<p; ++j){
            X.block<1,1>(i,j) << data->get(i,j)
        }
        // Eigen::RowVectorXf row = observations.get(Observations::COVARIATES, sampleID);
        // X.block<1,p>(i, 0) << row;
        Y(i) << observations.get(Observations::OUTCOME, sampleID);
        ++i;
    }
    
    // Identity matrix
    Eigen::Matrix<double, p, p> Id = Eigen::Matrix<double, p, p>::Identity();
    
    Eigen::Matrix<double, p, p> J = Id;
    J(0,0) = 0;
    
    // Pre-compute ridged variance estimate and its inverse
    Eigen::Matrix<double, p, p> M = X.transpose()*X + J*lambda;
    Eigen::Matrix<double, p, p> M_inverse = M.colPivHouseholderQr().solve(Id);
    
    theta = M*X.transpose()*Y;
    
    double rho;
    std::unordered_map<size_t, double> relabeled_observations;
    for (size_t sampleID : node_sampleIDs) {
        double response = observations.get(Observations::OUTCOME, sampleID);
        std::vector<double> xi = X.block<1,p>(sampleID,0);
        
        rho << M_inverse*(Y-theta*xi);
        relabeled_observations[sampleID] = rho;
    }
    return relabeled_observations;
}

