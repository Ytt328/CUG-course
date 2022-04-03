package com.example.demo.common;

public class Result {
    private String msg;//信息
    private double price;//价钱

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public Result() {
        this.price=0;
    }
}
