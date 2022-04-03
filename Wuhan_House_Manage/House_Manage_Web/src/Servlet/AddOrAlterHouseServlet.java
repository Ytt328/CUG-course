package Servlet;

import Entity.House;
import Function.Company_Manage;
import Function.Search;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "AddOrAlterHouseServlet", value = "/AddOrAlterHouseServlet")
public class AddOrAlterHouseServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");

        Object[] inf = new Object[11];
        inf[0] = request.getParameter("housetype");
        inf[1] = request.getParameter("price");
        inf[2] = request.getParameter("avgprice");
        inf[3] = request.getParameter("size");
        inf[4] = request.getParameter("innersize");
        inf[5] = request.getParameter("towards");
        inf[6] = request.getParameter("nname");
        inf[7] = request.getParameter("hfloor");
        inf[8] = request.getParameter("tfloor");
        inf[9] = request.getParameter("buildyear");
        inf[10] = request.getParameter("buildtype");
        for (int i = 0; i < 11; i++) {
            if (inf[i]!=null&&inf[i] != "") {
                inf[i] = "'" + inf[i] + "'";
            }
            else {
                if(i==0||i==1||i==2||i==3||i==6||i==9){
                    request.setAttribute("message", "必要条件不能为空！");
                    request.getRequestDispatcher("AddOrAlterHouse.jsp").forward(request, response);
                }else {
                    inf[i] = null;
                }
            }
        }
        //增加房源
        if(request.getParameter("id") == null|| request.getParameter("id").equals("")) {
            new Company_Manage().Insert(inf, "house");
            request.setAttribute("message", "插入成功！");
            request.getRequestDispatcher("AddOrAlterHouse.jsp").forward(request, response);
        }
        //修改房源
        else{
            int id= Integer.parseInt(request.getParameter("id"));
            Object[] inf1=new Object[1];
            inf1[0]=id;
            if(new Company_Manage().Update(inf1,inf)){
                request.setAttribute("message","修改成功！");
                House house=new Search().SearchHousebyID(id);
                request.setAttribute("house",house);
                request.getRequestDispatcher("AddOrAlterHouse.jsp").forward(request,response);
            }
            else{
                request.setAttribute("message","修改失败！该房源可能已经被删除，请刷新重试");
                House house=new Search().SearchHousebyID(id);
                request.setAttribute("house",house);
                request.getRequestDispatcher("AddOrAlterHouse.jsp").forward(request,response);
            }
        }
    }
}
