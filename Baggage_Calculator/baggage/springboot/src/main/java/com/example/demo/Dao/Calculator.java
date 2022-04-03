package com.example.demo.Dao;

import com.example.demo.common.Result;
import com.example.demo.entity.Baggage;
import com.example.demo.entity.Ticket;

//计算托运所需费用
public class Calculator {

    /**
     * 计算托运费用
     * @param ticket  机票信息
     * @param baggages 行李信息
     * @return 托运费用
     */
    public Result calculate(Ticket ticket, Baggage[] baggages){
        String[] routestype=ticket.getRoutetype();
        if(routestype.length==1){//routestype[0].equals("国内航线")
            return calculate_inland(ticket,baggages);
        }
        else{
            return calculate_internation(ticket,baggages);
        }
    }

    /**
     * 计算国内航线的托运费用
     * @param ticket 机票详情
     * @param baggages 行李信息
     * @return 托运费用及提示信息
     */
    public Result calculate_inland(Ticket ticket,Baggage[] baggages){
        Result result=new Result();
        double price=0;//托运费用
        String msg = null;//信息
        int free_1=0;//可携带随身行李件数
        double free_1_1;//可携带随身行李的重量限制（单位：kg）
        double free_2=0;//免费托运限额1——根据舱位（单位：kg）
        double free_3=0;//免费托运限额2——根据会员（单位：kg）
        int free_4=0;//可免费携带的残疾、伤、病旅客辅助设备(托运)件数
        //int free_5=0;//可免费托运的小型可放入客舱的辅助设备/手动轮椅件数
        int free_6=0;//可免费托运的婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅件数
        if(ticket.getCabintype().equals("头等舱")||ticket.getCabintype().equals("公务舱")){
            free_1=2;
            free_1_1=8;
        }
        else{
            free_1=1;
            free_1_1=5;
        }
        if(ticket.getPassenger().equals("成人票")||ticket.getPassenger().equals("儿童票")){//成人/儿童票
            if(ticket.getCabintype().equals("头等舱")) {
                free_2 = 40;
            }
            else if(ticket.getCabintype().equals("公务舱")){
                free_2=30;
            }
            else{
                free_2=20;
            }
        }
        else{//婴儿票
            free_2=10;
            free_6=1;
        }

        if(ticket.getIsdisable()){
            free_4=1;
            //free_5=1;
        }

        if(ticket.getVip().equals("凤凰知音终身白金卡、白金卡")){
            free_3=30;
        }
        else if(ticket.getVip().equals("凤凰知音金卡、银卡")||ticket.getVip().equals("星空联盟金卡")){
            free_3=20;
        }

        for (Baggage baggage : baggages) {
            //System.out.println("1"+baggage.getType()+"1");
            if(baggage.getType().equals("普通行李")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32) {
                    if (baggage.getLength() <= 55 && baggage.getWidth() <= 40 && baggage.getHeight() <= 20 && baggage.getWeight() <= free_1_1 && free_1 > 0) {
                        free_1--;
                    } else {
                        if (baggage.getWeight() <= free_2) {
                            free_2 = free_2 - baggage.getWeight();
                        } else {
                            if (baggage.getWeight() <= free_3) {
                                free_3 = 0;
                            } else {
                                price = price + (baggage.getWeight() - free_2) * (0.015 * ticket.getPrice());
                            }
                        }
                    }
                }
                else{
                    msg="存在普通行李大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("残疾、伤、病旅客辅助设备")) {
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32) {
                    if (ticket.getIsdisable() && free_4 == 1) {//是残疾、伤、病旅客，可免费
                        free_4--;
                    }
                    else {//托运
                        if(!ticket.getIsdisable()) {
                            if (baggage.getWeight() <= free_2) {
                                free_2 = free_2 - baggage.getWeight();
                            }
                            else {
                                price = price + (baggage.getWeight() - free_2) * (0.015 * ticket.getPrice());
                            }
                        }
                        else{
                            price = price + (baggage.getWeight()) * (0.015 * ticket.getPrice());
                        }
                    }
                }
                else{
                    msg="存在残疾、伤、病旅客辅助设备大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32) {
                    if (ticket.getPassenger().equals("婴儿票") && free_6 == 1) {
                        free_6--;
                    }
                    else {//需要托运
                        if(!ticket.getPassenger().equals("婴儿票")) {
                            if (baggage.getWeight() <= free_2) {
                                free_2 = free_2 - baggage.getWeight();
                            } else {
                                price = price + (baggage.getWeight() - free_2) * (0.015 * ticket.getPrice());
                            }
                        }
                        else{
                            price = price + (baggage.getWeight()) * (0.015 * ticket.getPrice());
                        }
                    }
                }
                else{
                    msg="存在婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("导盲犬/助听犬/精神抚慰犬")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){

                }
                else{
                    msg="存在导盲犬/助听犬/精神抚慰犬大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("骨灰")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){

                }
                else{
                    msg="存在骨灰大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("运动器械器材类1")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){
                    if (baggage.getWeight() <= free_2) {
                        free_2 = free_2 - baggage.getWeight();
                    }
                    else {
                        price = price + (baggage.getWeight() - free_2) * (0.015 * ticket.getPrice());
                    }
                }
                else{
                    msg="存在运动器械器材类1大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("运动器械器材类2")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32&&baggage.getWeight()>=2){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+2600;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+3900;
                    }
                    else if(baggage.getWeight()>32&&baggage.getWeight()<=45){
                        price=price+5200;
                    }
                }
                else{
                    msg="存在运动器械器材类2大小(过大或过小)不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("运动器械器材类3")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+1300;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+2600;
                    }
                    else if(baggage.getWeight()>32&&baggage.getWeight()<=45){
                        price=price+3900;
                    }
                }
                else{
                    msg="存在运动器械器材类3大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("睡袋/背包/野营用具/渔具/乐器")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){
                    if (baggage.getWeight() <= free_2) {
                        free_2 = free_2 - baggage.getWeight();
                    }
                    else {
                        price = price + (baggage.getWeight() - free_2) * (0.015 * ticket.getPrice());
                    }
                }
                else{
                    msg="存在睡袋/背包/野营用具/渔具/乐器大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("小型电器或仪器、媒体设备")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+490;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+3900;
                    }
                }
                else{
                    msg="存在小型电器或仪器、媒体设备大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("可作为行李运输的枪支")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+1300;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+2600;
                    }
                }
                else{
                    msg="存在可作为行李运输的枪支大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("可作为行李运输的子弹")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=5){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=5){
                        price=price+1300;
                    }
                }
                else{
                    msg="存在可作为行李运输的子弹大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("小动物(仅限家庭驯养的猫、狗)")){
                if(baggage.getLength()<=100&&baggage.getWidth()<=60&&baggage.getHeight()<=40&&baggage.getWeight()<=32){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=8){
                        price=price+3900;
                    }
                    else if(baggage.getWeight()>8&&baggage.getWeight()<=23){
                        price=price+5200;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+7800;
                    }
                }
                else{
                    msg="存在小动物(仅限家庭驯养的猫、狗)大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
        }
        result.setPrice(price);
        result.setMsg(msg);
        return result;
    }

    /**
     * 计算国际航线的托运费用
     * @param ticket 机票详情
     * @param baggages 行李信息
     * @return 托运费用及提示信息
     */
    public Result calculate_internation(Ticket ticket,Baggage[] baggages){
        Result result=new Result();
        double price = 0;//托运费用
        String msg=null;//提示信息
        int free_1=0;//可携带随身行李件数
        double free_1_1=0;//随身携带行李重量限制（kg）
        double free_2=0;//免费托运行李的重量限度（kg）
        int free_3=0;//免费托运行李的数量限度
        int free_4=0;//可免费携带的残疾、伤、病旅客辅助设备(托运)件数
        //int free_5=0;//可免费托运的小型可放入客舱的辅助设备/手动轮椅件数
        int free_6=0;//可免费托运的婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅件数
        String[] routetype=ticket.getRoutetype();
        String routetype1=routetype[1];//国际航线区域

        if(ticket.getCabintype().equals("头等、公务舱")){
            free_1=2;
            free_1_1=8;
        }
        else{
            free_1=1;
            free_1_1=5;
        }

        if(ticket.getCabintype().equals("头等、公务舱")){
            free_2=32;
            free_3=2;
        }
        else if(ticket.getCabintype().equals("悦享经济舱、超级经济舱")){
            free_2=23;
            free_3=2;
        }
        else if(ticket.getCabintype().equals("经济舱（区域A）")){
            free_2=23;
            free_3=1;
        }
        else{
            free_2=23;
            free_3=2;
        }

        if(ticket.getIsdisable()){
            free_4=1;
            //free_5=1;
        }
        if(ticket.getPassenger().equals("婴儿票")){
            free_6=1;
        }

        int num=0;//已超出的普通行李数
        for(Baggage baggage:baggages){
            if(baggage.getType().equals("普通行李")){
                if(baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(baggage.getWeight()<=free_2&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=158){
                        //不超重且不超尺寸
                        if(baggage.getLength()<=55&&baggage.getWidth()<=40&&baggage.getHeight()<=20&&free_1>0&&baggage.getWeight()<=free_1_1){
                            free_1--;//可作为随身行李
                        }
                        else {//托运
                            if (free_3 > 0) {//在免费运输范围内
                                free_3--;
                            } else {
                                num++;
                                price=price+calculate_internation_over1(routetype1,baggage,num);
                            }
                        }

                    }
                    if(baggage.getWeight()>free_2&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=158) {
                        //超重且不超尺寸
                        if (free_3 > 0) {//在免费运输范围内
                            free_3--;
                            price = price + calculate_internation_over2(routetype1, baggage, num);
                        } else {
                            num++;
                            price = price + calculate_internation_over2(routetype1, baggage, num);
                        }
                    }
                    if(baggage.getWeight()<=free_2&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())>158){
                        //不超重且超尺寸
                        if(free_3>0){//在免费运输范围内
                            free_3--;
                            price=price+calculate_internation_over3(routetype1,baggage,num);
                        }
                        else{
                            num++;
                            price=price+calculate_internation_over3(routetype1,baggage,num);
                        }
                    }
                    if(baggage.getWeight()>free_2&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())>158){
                        //超重且超尺寸
                        if(free_3>0){
                            free_3--;
                            price=price+calculate_internation_over4(routetype1,baggage,num);
                        }
                        else{
                            num++;
                            price=price+calculate_internation_over4(routetype1,baggage,num);
                        }
                    }
                }
                else{
                    msg="存在普通行李大小不满足托运行李要求，无法托运！";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("残疾、伤、病旅客辅助设备")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if (free_4 > 0) {
                        free_4--;//残疾、病、伤旅客可免费托运一件残疾、伤、病旅客辅助设备
                        if(baggage.getWeight()>free_2){
                            price=price+calculate_internation_over2(routetype1,baggage,num);
                        }
                    } else {
                        if (ticket.getIsdisable()) {//是残疾、病、伤旅客,不计入免费行李额
                            num++;
                            if(baggage.getWeight()<=free_2) {
                                price = price + calculate_internation_over1(routetype1, baggage, num);
                            }
                            else{
                                price = price + calculate_internation_over2(routetype1, baggage, num);
                            }
                        } else {//不是残疾旅客，计入免费行李额
                            if (free_3 > 0) {
                                free_3--;
                                if(baggage.getWeight()>free_2){
                                    price=price+calculate_internation_over2(routetype1,baggage,num);
                                }
                            } else {
                                num++;
                                if(baggage.getWeight()<=free_2) {
                                    price = price + calculate_internation_over1(routetype1, baggage, num);
                                }
                                else{
                                    price = price + calculate_internation_over2(routetype1, baggage, num);
                                }
                            }
                        }
                    }
                }
                else{
                    msg="存在残疾、伤、病旅客辅助设备 大小不满足托运行李要求，无法托运！";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(free_6>0){
                        free_6--;//婴儿可免费托运一辆婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅
                        if(baggage.getWeight()>free_2){
                            price=price+calculate_internation_over2(routetype1,baggage,num);
                        }
                    }
                    else{
                        if (ticket.getPassenger().equals("婴儿票")) {//是婴儿旅客,不计入免费行李额
                            num++;
                            if(baggage.getWeight()<=free_2) {
                                price = price + calculate_internation_over1(routetype1, baggage, num);
                            }
                            else{
                                price = price + calculate_internation_over2(routetype1, baggage, num);
                            }
                        } else {//不是婴儿旅客，计入免费行李额
                            if (free_3 > 0) {
                                free_3--;
                                if(baggage.getWeight()>free_2){
                                    price=price+calculate_internation_over2(routetype1,baggage,num);
                                }
                            } else {
                                num++;
                                if(baggage.getWeight()<=free_2) {
                                    price = price + calculate_internation_over1(routetype1, baggage, num);
                                }
                                else{
                                    price = price + calculate_internation_over2(routetype1, baggage, num);
                                }
                            }
                        }
                    }
                }
                else{
                    msg="存在婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅 大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("导盲犬/助听犬/精神抚慰犬")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){

                }
                else{
                    msg="存在导盲犬/助听犬/精神抚慰犬大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("骨灰")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){

                }
                else{
                    msg="存在骨灰大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("运动器械器材类1")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if (free_3 > 0) {
                        free_3--;
                        if(baggage.getWeight()>free_2){
                            price=price+calculate_internation_over2(routetype1,baggage,num);
                        }
                    } else {
                        num++;
                        if(baggage.getWeight()<=free_2) {
                            price = price + calculate_internation_over1(routetype1, baggage, num);
                        }
                        else{
                            price = price + calculate_internation_over2(routetype1, baggage, num);
                        }
                    }
                }
                else{
                    msg="存在运动器械器材类1 大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("运动器械器材类2")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+2600;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+3900;
                    }
                    else if(baggage.getWeight()>32&&baggage.getWeight()<=45){
                        price=price+5200;
                    }
                }
                else{
                    msg="存在运动器械器材类2大小(过大或过小)不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("运动器械器材类3")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+1300;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+2600;
                    }
                    else if(baggage.getWeight()>32&&baggage.getWeight()<=45){
                        price=price+3900;
                    }
                }
                else{
                    msg="存在运动器械器材类3大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("睡袋/背包/野营用具/渔具/乐器")){
                if (free_3 > 0) {
                    free_3--;
                    if(baggage.getWeight()>free_2){
                        price=price+calculate_internation_over2(routetype1,baggage,num);
                    }
                } else {
                    num++;
                    if(baggage.getWeight()<=free_2) {
                        price = price + calculate_internation_over1(routetype1, baggage, num);
                    }
                    else{
                        price = price + calculate_internation_over2(routetype1, baggage, num);
                    }
                }
            }
            if(baggage.getType().equals("小型电器或仪器、媒体设备")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+490;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+3900;
                    }
                }
                else{
                    msg="存在小型电器或仪器、媒体设备大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("可作为行李运输的枪支")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=23){
                        price=price+1300;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+2600;
                    }
                }
                else{
                    msg="存在可作为行李运输的枪支大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("可作为行李运输的子弹")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=5&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    price=price+1300;
                }
                else{
                    msg="存在可作为行李运输的子弹大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
            if(baggage.getType().equals("小动物(仅限家庭驯养的猫、狗)")){
                if(baggage.getWeight()>=2&&baggage.getWeight()<=32&&(baggage.getLength()+baggage.getWidth()+baggage.getHeight())<=203){
                    if(baggage.getWeight()>=2&&baggage.getWeight()<=8){
                        price=price+3900;
                    }
                    else if(baggage.getWeight()>8&&baggage.getWeight()<=23){
                        price=price+5200;
                    }
                    else if(baggage.getWeight()>23&&baggage.getWeight()<=32){
                        price=price+7800;
                    }
                }
                else{
                    msg="存在小动物(仅限家庭驯养的猫、狗)大小不满足托运行李要求，无法托运!";
                    result.setMsg(msg);
                    return result;
                }
            }
        }

        result.setPrice(price);
        result.setMsg(msg);
        return result;
    }

    /**
     * 国际航线各区域 不超重且不超尺寸 需要的费用
     * @param routetype1 航线区域
     * @param baggage 行李详情
     * @param num 超出的行李件数
     * @return 需要的费用
     */
    public double calculate_internation_over1(String routetype1,Baggage baggage,int num){
        double price=0;
        if(num==0){//未超出
                price=0;
                return price;
            }
        else if (num == 1) {//超出的第一件
                if (routetype1.equals("区域一")) {
                    price = 1400;
                } else if (routetype1.equals("区域二")) {
                    price = 1100;
                } else if (routetype1.equals("区域三")) {
                    price = 1170;
                } else if (routetype1.equals("区域四")) {
                    price = 1180;
                } else {
                    price = 830;
                }
                return price;
            }
        else if (num == 2) {//超出的第二件
                if (routetype1.equals("区域一")) {
                    price = 2000;
                } else if (routetype1.equals("区域二")) {
                    price = 1100;
                } else if (routetype1.equals("区域三")) {
                    price = 1170;
                } else if (routetype1.equals("区域四")) {
                    price = 1180;
                } else {
                    price = 1100;
                }
                return price;
            }
        else {//超出的第三件及以上
                if (routetype1.equals("区域一")) {
                    price = 3000;
                } else {
                    price = 1590;
                }
                return price;
            }
    }

    /**
     * 国际航线各区域 超重而不超尺寸 需要的费用
     * @param routetype1 航线区域
     * @param baggage 行李详情
     * @param num 超出的行李件数
     * @return 需要的费用
     */
    public double calculate_internation_over2(String routetype1,Baggage baggage,int num){
        double price=0;
        if(baggage.getWeight()>23&&baggage.getWeight()<=28){//超重1
            if(num==0){//在免费运输范围内
                if(routetype1.equals("区域一")){
                    price=380;
                }
                else if(routetype1.equals("区域二")){
                    price=280;
                }
                else if(routetype1.equals("区域三")){
                    price=520;
                }
                else if(routetype1.equals("区域四")){
                    price=690;
                }
                else{
                    price=210;
                }
                return price;
            }
            else if(num==1) {//第一次超出
                if (routetype1.equals("区域一")) {
                    price = 380 + 1400;
                } else if (routetype1.equals("区域二")) {
                    price = 280 + 1100;
                } else if (routetype1.equals("区域三")) {
                    price = 520 + 1170;
                } else if (routetype1.equals("区域四")) {
                    price = 690 + 1380;
                } else {
                    price = 210 + 830;
                }
                return price;
            }
            else if(num==2) {//第二次超出
                if (routetype1.equals("区域一")) {
                    price =  380 + 2000;
                } else if (routetype1.equals("区域二")) {
                    price =  280 + 1100;
                } else if (routetype1.equals("区域三")) {
                    price =  520 + 1170;
                } else if (routetype1.equals("区域四")) {
                    price =  690 + 1380;
                } else {
                    price =  210 + 1100;
                }
                return price;
            }
            else {//第三次及以上超出
                if (routetype1.equals("区域一")) {
                    price = 380 + 3000;
                } else if (routetype1.equals("区域二")) {
                    price = 280 + 1590;
                } else if (routetype1.equals("区域三")) {
                    price = 520 + 1590;
                } else if (routetype1.equals("区域四")) {
                    price = 690 + 1590;
                } else {
                    price = 210 + 1590;
                }
                return price;
            }
        }
        else{//超重2
            if(num==0){//在免费运输范围内
                if(routetype1.equals("区域一")){
                    price=980;
                }
                else if(routetype1.equals("区域二")){
                    price=690;
                }
                else if(routetype1.equals("区域三")){
                    price=520;
                }
                else if(routetype1.equals("区域四")){
                    price=1040;
                }
                else{
                    price=520;
                }
                return price;
            }
            else if(num==1) {//第一次超出
                if (routetype1.equals("区域一")) {
                    price = 980 + 1400;
                } else if (routetype1.equals("区域二")) {
                    price = 690 + 1100;
                } else if (routetype1.equals("区域三")) {
                    price = 520 + 1170;
                } else if (routetype1.equals("区域四")) {
                    price = 1040 + 1380;
                } else {
                    price = 520 + 830;
                }
                return price;
            }
            else if(num==2) {//第二次超出
                if (routetype1.equals("区域一")) {
                    price = 980 + 2000;
                } else if (routetype1.equals("区域二")) {
                    price = 690 + 1100;
                } else if (routetype1.equals("区域三")) {
                    price = 520 + 1170;
                } else if (routetype1.equals("区域四")) {
                    price = 1040 + 1380;
                } else {
                    price = 520 + 1100;
                }
                return price;
            }
            else {//第三次及以上超出
                if (routetype1.equals("区域一")) {
                    price = 980 + 3000;
                } else if (routetype1.equals("区域二")) {
                    price = 690 + 1590;
                } else if (routetype1.equals("区域三")) {
                    price = 520 + 1590;
                } else if (routetype1.equals("区域四")) {
                    price = 1040 + 1590;
                } else {
                    price = 520 + 1590;
                }
                return price;
            }
        }
    }

    /**
     * 国际航线各区域 不超重而超尺寸 需要的费用
     * @param routetype1 航线区域
     * @param baggage 行李详情
     * @param num 超出的行李件数
     * @return 需要的费用
     */
    public double calculate_internation_over3(String routetype1,Baggage baggage,int num){
        double price=0;
        if(num==0) {//在免费运输范围内
            if (routetype1.equals("区域一")) {
                price = 980;
            } else if (routetype1.equals("区域二")) {
                price = 690;
            } else if (routetype1.equals("区域三")) {
                price = 520;
            } else if (routetype1.equals("区域四")) {
                price = 1040;
            } else {
                price = 520;
            }
            return price;
        }
        else if(num==1) {
            if (routetype1.equals("区域一")) {
                price = 980 + 1400;
            } else if (routetype1.equals("区域二")) {
                price = 690 + 1100;
            } else if (routetype1.equals("区域三")) {
                price = 520 + 1170;
            } else if (routetype1.equals("区域四")) {
                price = 1040 + 1380;
            } else {
                price = 520 + 830;
            }
            return price;
        }
        else if(num==2) {
            if (routetype1.equals("区域一")) {
                price = 980 + 2000;
            } else if (routetype1.equals("区域二")) {
                price = 690 + 1100;
            } else if (routetype1.equals("区域三")) {
                price = 520 + 1170;
            } else if (routetype1.equals("区域四")) {
                price = 1040 + 1380;
            } else {
                price = 520 + 1100;
            }
            return price;
        }
        else {
            if (routetype1.equals("区域一")) {
                price = 980 + 3000;
            } else if (routetype1.equals("区域二")) {
                price = 690 + 1590;
            } else if (routetype1.equals("区域三")) {
                price = 520 + 1590;
            } else if (routetype1.equals("区域四")) {
                price = 1040 + 1590;
            } else {
                price = 520 + 1590;
            }
            return price;
        }
    }

    /**
     * 国际航线各区域 超重且超尺寸 需要的费用
     * @param routetype1 航线区域
     * @param baggage 行李详情
     * @param num 超出的行李件数
     * @return 需要的费用
     */
    public double calculate_internation_over4(String routetype1,Baggage baggage,int num){
        double price=0;
        if(num==0) {
            if (routetype1.equals("区域一")) {
                price = 1400;
            } else if (routetype1.equals("区域二")) {
                price = 1100;
            } else if (routetype1.equals("区域三")) {
                price = 520;
            } else if (routetype1.equals("区域四")) {
                price = 2050;
            } else {
                price = 830;
            }
            return price;
        }
        else if(num==1) {
            if (routetype1.equals("区域一")) {
                price = 1400 + 1400;
            } else if (routetype1.equals("区域二")) {
                price = 1100 + 1100;
            } else if (routetype1.equals("区域三")) {
                price = 520 + 1170;
            } else if (routetype1.equals("区域四")) {
                price = 2050 + 1380;
            } else {
                price = 830 + 830;
            }
            return price;
        }
        else if(num==2) {
            if (routetype1.equals("区域一")) {
                price = 1400 + 2000;
            } else if (routetype1.equals("区域二")) {
                price = 1100 + 1100;
            } else if (routetype1.equals("区域三")) {
                price = 520 + 1170;
            } else if (routetype1.equals("区域四")) {
                price = 2050 + 1380;
            } else {
                price = 830 + 1100;
            }
            return price;
        }
        else {
            if (routetype1.equals("区域一")) {
                price = 1400 + 3000;
            } else if (routetype1.equals("区域二")) {
                price = 1100 + 1590;
            } else if (routetype1.equals("区域三")) {
                price = 520 + 1590;
            } else if (routetype1.equals("区域四")) {
                price = 2050 + 1590;
            } else {
                price = 830 + 1590;
            }
            return price;
        }
    }
}
