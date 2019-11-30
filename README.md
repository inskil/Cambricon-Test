# Cambricon-Test

Cambricon-Test for BANG as a homework

## 问题描述

[描述文档](https://github.com/inskil/Cambricon-Test/blob/master/bang_practice/problem_set.md)

## 解答

### bangConsine

#### 代码

[项目地址](https://github.com/inskil/Cambricon-Test/tree/master/bang_practice/bangCosine)

核心代码：

```c {.line-numbers}
// calculate sum (Xi * Yi)
for (int32_t i = 0; i < M; i++)
{
    // GET LINE DATA INTO CAL NRAM
    __memcpy(calX_nram, inputX_nram + i * ONELINE, ONELINE * sizeof(half), NRAM2NRAM);
    __memcpy(calY_nram, inputY_nram + i * ONELINE, ONELINE * sizeof(half), NRAM2NRAM);

    // __bang_mul_const(calX_nram, calX_nram, 0.005, ONELINE);
    // __bang_mul_const(calY_nram, calY_nram, 0.005, ONELINE);

    // XY
    __bang_mul(temp1_nram, calX_nram, calY_nram, ONELINE);     // x*y
    __bang_add(tempXY_nram, temp1_nram, tempXY_nram, ONELINE); // result +=  x*y

    // XX
    __bang_mul(temp2_nram, calX_nram, calX_nram, ONELINE);     // x*x
    __bang_add(tempXX_nram, temp2_nram, tempXX_nram, ONELINE); // result +=  x*x

    // YY
    __bang_mul(temp3_nram, calY_nram, calY_nram, ONELINE);     // y*y
    __bang_add(tempYY_nram, temp3_nram, tempYY_nram, ONELINE); // result +=  y*y
}

// XX=1/(power1/2,Xx)
__bang_add(tempXX_nram, tempXX_nram, esp_nram, ONELINE);
__bang_active_sqrt(tempXX_nram, tempXX_nram, ONELINE);
__bang_active_recip(tempXX_nram, tempXX_nram, ONELINE);
// YY=1/(power1/2,YY)
__bang_add(tempYY_nram, tempYY_nram, esp_nram, ONELINE);
__bang_active_sqrt(tempYY_nram, tempYY_nram, ONELINE);
__bang_active_recip(tempYY_nram, tempYY_nram, ONELINE);

__bang_mul(temp2_nram, tempYY_nram, tempXX_nram, ONELINE); // 1 / XX * YY

__bang_mul(temp3_nram, tempXY_nram, temp2_nram, ONELINE); // XY / XX*
```

#### 结果

    M: 256
    N: 256
    LEN: 65536
    +++++++++++++++++test1++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.243 ms
    _________this is test____________
    cpu sum :0.000010
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%
    +++++++++++++++++test2++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.243 ms
    _________this is test____________
    cpu sum :256.000000
    Total error:0.500000, MAE:0.000008
    err rate = 0.1953%
    +++++++++++++++++test3++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.243 ms
    _________this is test____________
    cpu sum :15.292888
    Total error:0.123910, MAE:0.000002
    err rate = 0.8102%
    +++++++++++++++++test4++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.243 ms
    _________this is test____________
    cpu sum :256.000000
    Total error:0.500000, MAE:0.000008
    err rate = 0.1953%
    +++++++++++++++++test5++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.243 ms
    _________this is test____________
    cpu sum :0.000010
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%
    +++++++++++++++++test6++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.243 ms
    _________this is test____________
    cpu sum :187.375519
    Total error:1.772051, MAE:0.000027
    err rate = 0.9457%

### bangBN

#### 代码

[项目地址](https://github.com/inskil/Cambricon-Test/tree/master/bang_practice/bangBN)

核心代码：

```c {.line-numbers}
// tans
__bang_transpose(tans_nram, input_nram, M, N);

// add SUM(X)
for (int32_t i = 0; i < N; i++)
{
    __bang_add(temp1_nram, tans_nram + i * M, temp1_nram, M);
}
// AVG = sum(x) / N
__bang_mul_const(avg_nram, temp1_nram, REN, M);

//variance
// SUM((X-AVG)^2)
for (int32_t i = 0; i < N; i++)
{
    __bang_sub(temp1_nram, tans_nram + i * M, avg_nram, M); // X  - AVG
    __bang_mul(temp1_nram, temp1_nram, temp1_nram, M);      // (X-AVG)^2
    __bang_add(temp2_nram, temp1_nram, temp2_nram, M);      // temp2 = SUM((X-AVG)^2)
}
// var = temp2 / N
__bang_mul_const(var_nram, temp2_nram, REN, M);

// Standardization x

__bang_add(temp2_nram, var_nram, esp_nram, M);  // temp2 = var + esp
__bang_active_sqrt(temp2_nram, var_nram, M);    // temp2 = sqrt(var + esp)
__bang_active_recip(temp2_nram, temp2_nram, M); // temp2 = 1/temp2 = 1/sqrt(var + esp)

for (int32_t i = 0; i < N; i++)
{
    __bang_sub(temp1_nram, tans_nram + i * M, avg_nram, M);  // temp1 = X  - AVG
    __bang_mul(sta_nram + i * M, temp1_nram, temp2_nram, M); // sta = temp1 * temp2 = X  - AVG / sqrt(var + esp)
}

// result = 2.0 * staX + beta
__bang_mul_const(result_nram, sta_nram, 1.957, LEN); // 2.0
__bang_add(result_nram, result_nram, beta_nram, LEN); // RESULT + =beta

// tans return
__bang_transpose(tans_nram, result_nram, N, M);
```

#### 结果

    M: 128
    N: 256
    LEN: 32768
    ++++++++++++++++test1+++++++++++++++++++CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.218 ms
    ---cpu---
    Total error:171.333694, MAE:0.005229
    err rate = 0.2956%
    ++++++++++++++++test2+++++++++++++++++++CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.219 ms
    ---cpu---
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%
    ++++++++++++++++testfile+++++++++++++++++++CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.218 ms
    ---cpu---
    Total error:921.975342, MAE:0.028136
    err rate = 1.2905%

### bangTripleloss

#### 代码

[项目地址](https://github.com/inskil/Cambricon-Test/tree/master/bang_practice/bangTripletloss)

核心代码：

```c {.line-numbers}
for (int32_t i = 0; i < M; i++)
{

    __memcpy(inputA_nram , inputA + i * N, N * sizeof(half), GDRAM2NRAM);
    __memcpy(inputP_nram , inputP + i * N, N * sizeof(half), GDRAM2NRAM);
    __memcpy(inputN_nram , inputN + i * N, N * sizeof(half), GDRAM2NRAM);

    // __bang_mul_const(calX_nram, calX_nram, 0.005, ONELINE);

    __bang_sub(tempa_nram, inputA_nram, inputP_nram, ONELINE); // A-P
    __bang_abs(tempa_nram);  //  | A - P |
    __bang_add(adda_nram, adda_nram, tempa_nram, ONELINE); // adda = sum (| A - P |)

    __bang_sub(tempb_nram, inputA_nram, inputN_nram, ONELINE); // A-N
    __bang_abs(tempb_nram);  //  | A - N |
    __bang_add(addb_nram, addb_nram, tempb_nram, ONELINE); // addb = sum (| A - N |)

}

__bang_sub(tempa_nram, adda_nram,addb_nram, ONELINE); // tempa = adda - addb
__bang_add(tempb_nram, tempa_nram, const_nram, ONELINE); // tempa += margin

// if <0 change it to 0
    for (int32_t i = 0; i < N; i++)
{
    if (tempb_nram[i]<0)
    {
        tempb_nram[i] = 0 ;
    }
}

```

#### 结果

    M: 128
    N: 256
    LEN: 32768
    +++++++++++++++++test1++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.137 ms
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%

    +++++++++++++++++test2++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.137 ms
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%

    +++++++++++++++++test3++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.138 ms
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%

    +++++++++++++++++test4++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.137 ms
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%

    +++++++++++++++++test5++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.136 ms
    Total error:0.000000, MAE:0.000000
    err rate = 0.0000%

    +++++++++++++++++test6++++++++++++++++++++
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 0.138 ms
    Total error:1.975058, MAE:0.000060
    err rate = 0.9851%

### bangPowerXY

#### 代码

[项目地址](https://github.com/inskil/Cambricon-Test/tree/master/bang_practice/bangPowerXY)

核心代码：

```c {.line-numbers}
__memcpy(inputX_nram, inputX + i * ONELINE, ONELINE * sizeof(half), GDRAM2NRAM);
__memcpy(inputY_nram, inputY + i * ONELINE, ONELINE * sizeof(half), GDRAM2NRAM);
__nramset_half(temp1_nram, ONELINE, 7.0); //t0=0

for (int32_t j = 1; j <= ITER_NUM;j++) {
    __bang_mul_const(temp2_nram, temp1_nram, -1.0, ONELINE);  // -tn
    __bang_active_exp(temp2_nram, temp2_nram, ONELINE);       // exp(-tn)
    __bang_mul(temp2_nram, inputX_nram, temp2_nram, ONELINE);  // x*exp(-tn)
    __bang_add(temp1_nram, temp1_nram, temp2_nram, ONELINE);  // tn + x*exp(-tn)
    __bang_sub(temp1_nram, temp1_nram, const_nram, ONELINE);  // tn + x*exp(-tn) -1
}

__bang_mul(temp1_nram, temp1_nram, inputY_nram, ONELINE);            // term = inputY * ln(inputX)
__bang_active_exp(temp1_nram, temp1_nram, ONELINE);               // result = exp(term)

```

#### 结果

    M: 128
    N: 256
    LEN: 32768
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 189.127 ms
    MAE = 0.2459
    err rate = 0.9516%
    CNRT: 3.12.0 7bfac76
    Hardware Total Time: 189.125 ms
    MAE = 0.0156
    err rate = 0.3906%

## 代码优化
