package com.example.demo.entity;

import lombok.Data;

@Data
public class Ticket {
    String[] routetype;//航线类型
    String cabintype;//机舱类型
    String passenger;//乘客类型
    String vip;//会员类型
    double price;//机票价格
    boolean isdisable;//是否伤、残、病

    public Ticket(String[] routetype,String cabintype,String passenger,String vip,double price,boolean isdisable){
        this.routetype=routetype;
        this.cabintype=cabintype;
        this.passenger=passenger;
        this.vip=vip;
        this.price=price;
        this.isdisable=isdisable;
    }

    public void Set(String[] routetype,String cabintype,String passenger,String vip,double price,boolean isdisable){
        this.routetype=routetype;
        this.cabintype=cabintype;
        this.passenger=passenger;
        this.vip=vip;
        this.price=price;
        this.isdisable=isdisable;
    }

    public boolean getIsdisable(){
        return  isdisable;
    }
}


