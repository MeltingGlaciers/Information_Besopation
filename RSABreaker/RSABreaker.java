package com.company;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static java.lang.Integer.parseInt;

public class RSABreaker {

    public static String breaker(BigInteger n, BigInteger e, BigInteger message){

        BigInteger phi = findPhi(n);
        System.out.println("phi= "+phi);

        BigInteger d = e.modInverse(phi);
        System.out.println("d= "+d);

        List<BigInteger> c = new ArrayList<>();
        String messageStr = message.toString(); //делать срезы удобнее со строками
        int lenN = n.toString().length();


        /*
        * Деление числа происходит по принципу:
        * 1. берем часть с длиной = длины n
        * 2. если она больше n, то берем часть длиной - 1
        * 3. часть запоминаем, из строки ее отсекаем
        * 4. повторяем, пока длина остатка > длины n
        * 5. если что-то осталось - запоминаем (это будет априори < длины n
         */

        while (lenN<messageStr.length()){

            String curr = messageStr.substring(0,lenN);
            if (new BigInteger(curr).compareTo(n)>0) curr = messageStr.substring(0,lenN-1);
            c.add(new BigInteger(curr));
            messageStr = messageStr.substring(curr.length(),messageStr.length());

        }

        if (messageStr.length()!=0)
            c.add(new BigInteger(messageStr));

        System.out.println(c);

        List<BigInteger> m = c.stream().map(p->p.modPow(d,n)).collect(Collectors.toList());
        // к каждому блоку применяем функцию расшифровки

        System.out.println(m);

        String allM = ""; //из массива блоков делаем одно большое число (пока что в виде строки)

        for (BigInteger v : m){

            allM = allM.concat(v.toString());

        }

        StringBuilder res = new StringBuilder();

        for (int i=0;i<allM.length();i+=2) {

            int a = parseInt(allM.substring(i, i + 2));
            res.append((char) a);
            //res.append(" ");

        }
        //берем по две цифры и переводим их из int в char. Результат записываем

        //System.out.println(res);

        return res.toString();

    }

    private static BigInteger findPhi(BigInteger n){

        BigInteger p = new BigInteger("2");
        BigInteger q = null;

        while(true){

            if ( n.mod(p).equals(BigInteger.ZERO)){

                q = n.divide(p);
                break;

            }

            p = p.add(BigInteger.ONE);

        }

        return p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));



    }

}
