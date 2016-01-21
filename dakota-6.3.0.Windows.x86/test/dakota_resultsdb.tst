Test Number 0 succeeded
<<<<< Function evaluation summary: 39 total (39 new, 0 duplicate)
<<<<< Best parameters          =
                     1.0013529993e+000 x1
                     1.0026805414e+000 x2
<<<<< Best objective function  =
                     1.9050696338e-006
<<<<< Best data captured at function evaluation 38
Test Number 1 succeeded
<<<<< Function evaluation summary: 200 total (200 new, 0 duplicate)
Sample moment statistics for each response function:
                            Mean           Std Dev          Skewness          Kurtosis
 response_fn_1 4.5540183516e+002 5.3682678089e+002 1.6661798252e+000 2.7925726822e+000
95% confidence intervals for each response function:
                    LowerCI_Mean      UpperCI_Mean    LowerCI_StdDev    UpperCI_StdDev
 response_fn_1 3.8054757609e+002 5.3025609422e+002 4.8886795789e+002 5.9530059589e+002
          Bin Lower          Bin Upper      Density Value
          ---------          ---------      -------------
  1.1623549854e-001  1.0000000000e+002  3.4039566059e-003
  1.0000000000e+002  2.7101710856e+003  2.5285698843e-004
     Response Level  Probability Level  Reliability Index  General Rel Index
     --------------  -----------------  -----------------  -----------------
  1.0000000000e+002  3.4000000000e-001
Simple Correlation Matrix among all inputs and outputs:
                       x1           x2 response_fn_1 
          x1 1.00000e+000 
          x2 -5.85097e-003 1.00000e+000 
response_fn_1 -9.57746e-002 -5.08193e-001 1.00000e+000 
Partial Correlation Matrix between input and output:
             response_fn_1 
          x1 -1.14659e-001 
          x2 -5.11111e-001 
Simple Rank Correlation Matrix among all inputs and outputs:
                       x1           x2 response_fn_1 
          x1 1.00000e+000 
          x2 -6.03315e-003 1.00000e+000 
response_fn_1 -1.15360e-001 -5.04661e-001 1.00000e+000 
Partial Rank Correlation Matrix between input and output:
             response_fn_1 
          x1 -1.37154e-001 
          x2 -5.08762e-001 
Test Number 2 succeeded
<<<<< Function evaluation summary: 15 total (15 new, 0 duplicate)
Coefficients of Polynomial Chaos Expansion for response_fn_1:
  4.5566666667e+002   P0   P0
  -4.0000000000e+000   P1   P0
  9.1695238095e+002   P2   P0
  -5.5955240441e-014   P3   P0
  3.6571428571e+002   P4   P0
  -5.3333333333e+002   P0   P1
  -4.7961634664e-014   P1   P1
  -1.0666666667e+003   P2   P1
  -8.3932860662e-014   P3   P1
  1.1151080059e-012   P4   P1
  2.6666666667e+002   P0   P2
  6.6613381478e-014   P1   P2
  2.2204460493e-013   P2   P2
  7.7715611724e-015   P3   P2
  -4.9960036108e-013   P4   P2
Moment statistics for each response function:
                            Mean           Std Dev          Skewness          Kurtosis
  expansion:   4.5566666667e+002 6.0656024184e+002
  integration: 4.5566666667e+002 6.0656024184e+002 1.9633285271e+000 3.3633861456e+000
response_fn_1 Sobol' indices:
                                  Main             Total
                     4.9746891383e-001 7.0363551328e-001 x1
                     2.9636448672e-001 5.0253108617e-001 x2
                     2.0616659946e-001 x1 x2 
          Bin Lower          Bin Upper      Density Value
          ---------          ---------      -------------
  6.8311107121e-003  1.0000000000e-001  2.0393073423e-002
  1.0000000000e-001  1.0000000000e+000  1.3000000000e-002
  1.0000000000e+000  5.0000000000e+001  4.7000000000e-003
  5.0000000000e+001  1.0000000000e+002  1.9680000000e-003
  1.0000000000e+002  5.0000000000e+002  9.2150000000e-004
  5.0000000000e+002  1.0000000000e+003  2.8300000000e-004
  1.0000000000e+003  3.5755437782e+003  5.7308286215e-005
     Response Level  Probability Level  Reliability Index  General Rel Index
     --------------  -----------------  -----------------  -----------------
  1.0000000000e-001  1.9000000000e-003
  1.0000000000e+000  1.3600000000e-002
  5.0000000000e+001  2.4390000000e-001
  1.0000000000e+002  3.4230000000e-001
  5.0000000000e+002  7.1090000000e-001
  1.0000000000e+003  8.5240000000e-001
