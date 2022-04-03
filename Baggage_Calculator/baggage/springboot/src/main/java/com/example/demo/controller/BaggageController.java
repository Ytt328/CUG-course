package com.example.demo.controller;

import com.example.demo.Dao.Calculator;
import com.example.demo.common.Result;
import com.example.demo.entity.Baggage;
import com.example.demo.entity.Ticket;
import com.example.demo.entity.Ticket_Baggage;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/calculate")
public class BaggageController {

    @PostMapping
    public Result calculate(@RequestBody Ticket_Baggage ticket_baggage){
        Ticket ticket= ticket_baggage.getTicket();//机票详情
        Result result = new Result();
        if(ticket.getPrice()<=0){
            result.setMsg("请输入正确的机票价钱！");
            return result;
        }
        Baggage[] baggages= ticket_baggage.getTableData();//行李详情
        for(Baggage baggage:baggages){
            if(baggage.getLength()<=0||baggage.getWidth()<=0||baggage.getHeight()<=0||baggage.getWeight()<=0){
                result.setMsg("请输入正确的行李尺寸！");
                return result;
            }
        }
        result=new Calculator().calculate(ticket,baggages);
        return result;
    }
}
