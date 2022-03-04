package com.company;

import java.math.BigInteger;

public class Main {

    public static void main(String[] args) {

//        BigInteger m = new BigInteger("4472019868828421289843038617813192879612275852133249779907137882545473750");
//        BigInteger n = new BigInteger("274607103517687");
//        BigInteger e = new BigInteger("11119");
        BigInteger m = new BigInteger("403013074606912545180648978557219641194372024501606729868202878976557455422");
        BigInteger n = new BigInteger("889577666850907");
        BigInteger e = new BigInteger("13971");

        System.out.println(RSABreaker.breaker(n,e,m));

    }
}
