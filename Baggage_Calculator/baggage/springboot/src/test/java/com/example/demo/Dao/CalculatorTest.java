package com.example.demo.Dao;

import com.example.demo.common.Result;
import com.example.demo.entity.Baggage;
import com.example.demo.entity.Ticket;
import org.junit.Assert;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class CalculatorTest {
    Calculator calculator;
    Result result;

    @BeforeEach
    void setUp() {
        calculator=new Calculator();
        result=new Result();
    }

    @AfterEach
    void tearDown() {
    }

    @Test
    void calculate_inland1() {//测试模块一：国内航线，普通行李
        System.out.println("测试模块一：国内航线——普通行李");
        System.out.print("testcase1:");
        String[] routetype=new String[1];
        routetype[0]="国内航线";
        Ticket ticket=new Ticket(routetype,"头等舱","成人票","凤凰知音终身白金卡、白金卡",500,false);
        Baggage[] baggages=new Baggage[1];
        Baggage baggage=new Baggage("普通行李",120,40,40,30);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertEquals(result.getMsg(),"存在普通行李大小不满足托运行李要求，无法托运!");
        System.out.println("pass");

        System.out.print("testcase2:");
        baggage.Set("普通行李",55,40,40,8);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase3:");
        baggage.Set("普通行李",60,40,40,20);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase4:");
        baggage.Set("普通行李",60,40,40,30);
        ticket.Set(routetype,"经济舱","成人票","凤凰知音终身白金卡、白金卡",500,false);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase5:");
        baggage.Set("普通行李",60,40,40,32);
        ticket.Set(routetype,"经济舱","成人票","凤凰知音终身白金卡、白金卡",500,false);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),90,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");
    }

    @Test
    void calculate_inland2(){//测试模块二：国内航线，残疾、伤、病旅客辅助设备
        System.out.println("测试模块二：国内航线——残疾、伤、病旅客辅助设备");
        System.out.print("testcase1:");
        String[] routetype=new String[1];
        routetype[0]="国内航线";
        Ticket ticket=new Ticket(routetype,"公务舱","成人票","无",500,true);
        Baggage[] baggages=new Baggage[1];
        Baggage baggage=new Baggage("残疾、伤、病旅客辅助设备",120,40,40,30);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertEquals(result.getMsg(),"存在残疾、伤、病旅客辅助设备大小不满足托运行李要求，无法托运!");
        System.out.println("pass");

        System.out.print("testcase2:");
        baggage.Set("残疾、伤、病旅客辅助设备",100,40,40,30);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase3:");
        Baggage[] baggages1=new Baggage[2];
        baggages1[0]=baggage;
        baggages1[1]=baggage;
        result=calculator.calculate_inland(ticket,baggages1);
        assertEquals(result.getPrice(),225,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase4:");
        ticket.setIsdisable(false);
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase5:");
        ticket.Set(routetype,"经济舱","成人票","无",500,false);
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),75,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

    }

    @Test
    void calculate_internation() {//测试模块三：国际航线：普通行李
        System.out.println("测试模块三：国际航线——普通行李");
        System.out.print("testcase1:");
        String[] routetype=new String[2];
        routetype[0]="国际航线";
        routetype[1]="区域一";
        Ticket ticket=new Ticket(routetype,"头等、公务舱","成人票","",500,false);
        Baggage baggage=new Baggage("普通行李",120,40,40,40);
        Baggage[] baggages=new Baggage[1];
        baggages[0]=baggage;
        result=calculator.calculate_internation(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertEquals(result.getMsg(),"存在普通行李大小不满足托运行李要求，无法托运！");
        System.out.println("pass");

        System.out.print("testcase2:");
        baggage.Set("普通行李",55,40,40,8);
        baggages[0]=baggage;
        result=calculator.calculate_inland(ticket,baggages);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase3:");
        baggage.Set("普通行李",60,40,20,8);
        Baggage[] baggages1=new Baggage[3];
        baggages1[0]=baggage;
        baggages1[1]=baggage;
        baggages1[2]=baggage;
        result=calculator.calculate_inland(ticket,baggages1);
        assertEquals(result.getPrice(),30,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase4:");
        baggage.Set("普通行李",60,40,20,8);
        Baggage[] baggages2=new Baggage[2];
        baggages2[0]=baggage;
        baggages2[1]=baggage;
        result=calculator.calculate_inland(ticket,baggages2);
        assertEquals(result.getPrice(),0,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase5:");
        ticket.Set(routetype,"悦享经济舱、超级经济舱","成人票","",500,false);
        baggage.Set("普通行李",60,40,20,30);
        Baggage baggage1=new Baggage("普通行李",60,40,20,8);
        Baggage[] baggages3=new Baggage[3];
        baggages3[0]=baggage1;
        baggages3[1]=baggage1;
        baggages3[2]=baggage1;
        result=calculator.calculate_inland(ticket,baggages3);
        assertEquals(result.getPrice(),30,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase6:");
        Baggage[] baggages4=new Baggage[2];
        baggages4[0]=baggage1;
        baggages4[1]=baggage;
        result=calculator.calculate_inland(ticket,baggages4);
        assertEquals(result.getPrice(),135,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase7:");
        Baggage baggage2=new Baggage("普通行李",60,40,20,20);
        Baggage[] baggages5=new Baggage[3];
        baggages5[0]=baggage;
        baggages5[1]=baggage;
        baggages5[2]=baggage2;
        result=calculator.calculate_inland(ticket,baggages5);
        assertEquals(result.getPrice(),150,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase8:");
        Baggage[] baggages6=new Baggage[2];
        baggages6[0]=baggage;
        baggages6[1]=baggage2;
        result=calculator.calculate_inland(ticket,baggages6);
        assertEquals(result.getPrice(),75,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase9:");
        Baggage baggage3=new Baggage("普通行李",80,60,40,30);
        Baggage[] baggages7=new Baggage[3];
        baggages7[0]=baggage;
        baggages7[1]=baggage;
        baggages7[2]=baggage3;
        result=calculator.calculate_inland(ticket,baggages7);
        assertEquals(result.getPrice(),225,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");

        System.out.print("testcase10:");
        Baggage[] baggages8=new Baggage[2];
        baggages8[0]=baggage;
        baggages8[1]=baggage3;
        result=calculator.calculate_inland(ticket,baggages8);
        assertEquals(result.getPrice(),150,0.001);
        assertNull(result.getMsg());
        System.out.println("pass");
    }
}