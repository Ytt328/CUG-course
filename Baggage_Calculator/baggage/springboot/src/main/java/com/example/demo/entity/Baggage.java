package com.example.demo.entity;

import lombok.Data;

@Data
public class Baggage {
    String type;//行李类型
    double length;//长度
    double width;//宽度
    double height;//高度
    double weight;//质量

    public Baggage(String type,double length,double width,double height,double weight){
        this.type=type;
        this.length=length;
        this.width=width;
        this.height=height;
        this.weight=weight;
    }

    public void Set(String type,double length,double width,double height,double weight){
        this.type=type;
        this.length=length;
        this.width=width;
        this.height=height;
        this.weight=weight;
    }
}
