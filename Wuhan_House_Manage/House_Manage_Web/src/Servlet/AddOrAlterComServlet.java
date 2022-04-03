package Servlet;

import Entity.Admin;
import Entity.Company;
import Function.Search;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "AddOrAlterComServlet", value = "/AddOrAlterComServlet")
public class AddOrAlterComServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");

        String name=request.getParameter("name");
        String password=request.getParameter("password");
        //添加企业用户
        if(request.getParameter("cname").equals("")){
            if(name==null||name.equals("")||password==null||password.equals("")){
                request.setAttribute("message", "不能输入空字段！");
                request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request, response);
            }
            else {
                if (new Admin().InsertCompany(name, password)) {
                    request.setAttribute("message", "添加成功");
                    request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request, response);
                } else {
                    request.setAttribute("message", "添加失败,请尝试修改用户名");
                    request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request, response);
                }
            }
        }
        //修改企业用户
        else{
            String namebefore=request.getParameter("cname");
            if(name==null||name.equals("")||password==null||password.equals("")){
                request.setAttribute("message", "不能输入空字段！");
                request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request, response);
            }
            else {
                if (new Admin().UpdateCompany(namebefore, name, password)) {
                    request.setAttribute("message", "修改成功");
                    Company company = new Search().SearchCompanybyname(name);
                    request.setAttribute("company", company);
                    request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request, response);
                } else {
                    request.setAttribute("message", "修改失败,请尝试修改账号");
                    Company company = new Search().SearchCompanybyname(namebefore);
                    request.setAttribute("company", company);
                    request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request, response);
                }
            }
        }
    }
}
