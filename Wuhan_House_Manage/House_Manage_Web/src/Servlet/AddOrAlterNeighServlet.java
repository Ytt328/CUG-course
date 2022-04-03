package Servlet;

import Entity.Neigh;
import Function.Company_Manage;
import Function.Search;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "AddOrAlterNeighServlet", value = "/AddOrAlterNeighServlet")
public class AddOrAlterNeighServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");

        Object[] inf=new Object[9];
        inf[0]=request.getParameter("longitude");
        inf[1]=request.getParameter("altitude");
        inf[2]=request.getParameter("xzname");
        inf[3]=request.getParameter("nname");
        inf[4]=request.getParameter("sales");
        inf[5]=request.getParameter("ntype");
        inf[6]=request.getParameter("price");
        inf[7]=request.getParameter("plot");
        inf[8]=request.getParameter("green");
        for(int i=0;i<9;i++){
            if(inf[i]!=null&&inf[i] != "") {
                inf[i]="'"+inf[i]+"'";
            }
            else{
                if(i==0||i==1||i==2||i==3||i==4||i==5){
                    request.setAttribute("message", "必要条件不能为空！");
                    request.getRequestDispatcher("AddOrAlterNeigh.jsp").forward(request, response);
                }
                else {
                    inf[i] = null;
                }
            }
        }
        //增加小区
        if(request.getParameter("nname_before")==null|| request.getParameter("nname_before").equals("")) {
            boolean flag = new Company_Manage().Insert(inf, "neighborhood");
            if (flag) {
                request.setAttribute("message", "插入成功！");
                request.getRequestDispatcher("AddOrAlterNeigh.jsp").forward(request, response);
            } else if (!flag) {
                request.setAttribute("message", "插入失败！失败原因:可能已存在小区名与小区类型相同的小区");
                request.getRequestDispatcher("AddOrAlterNeigh.jsp").forward(request, response);
            }
        }
        //修改小区
        else{
            String nname=request.getParameter("nname_before");
            Neigh neigh=new Search().SearchNeighbyNname(nname);
            Object[] inf1=new Object[2];
            inf1[0]=neigh.getNname();
            inf1[1]=neigh.getNtype();
            if(new Company_Manage().Update(inf1,inf)){
                request.setAttribute("message","修改成功！");
                String nameafter=inf[3].toString().replace("'","");
                Neigh neigh1=new Search().SearchNeighbyNname(nameafter);
                request.setAttribute("neigh",neigh1);
                request.getRequestDispatcher("AddOrAlterNeigh.jsp?").forward(request,response);
            }
            else{
                request.setAttribute("message","修改失败！该小区可能已经被删除，请刷新重试");
                Neigh neigh1=new Search().SearchNeighbyNname(nname);
                request.setAttribute("neigh",neigh1);
                request.getRequestDispatcher("AddOrAlterNeigh.jsp").forward(request,response);
            }
        }
    }
}
