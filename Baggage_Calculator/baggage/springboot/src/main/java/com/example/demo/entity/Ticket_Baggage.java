package com.example.demo.entity;

import lombok.Data;

@Data
public class Ticket_Baggage {
    Ticket ticket;
    Baggage[] tableData;
}
