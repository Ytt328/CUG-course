package Servlet;

import Entity.Admin;
import Entity.User;
import Function.Search;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "AddOrAlterUserServlet", value = "/AddOrAlterUserServlet")
public class AddOrAlterUserServlet extends HttpServlet {
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
        //添加用户
        if(request.getParameter("uname").equals("")){
            if(name==null||name.equals("")||password==null||password.equals("")){
                request.setAttribute("message", "不能输入空字段！");
                request.getRequestDispatcher("AddOrAlterUser.jsp").forward(request, response);
            }
            else {
                if (new Admin().InsertUser(name, password)) {
                    request.setAttribute("message", "添加成功");
                    request.getRequestDispatcher("AddOrAlterUser.jsp?").forward(request, response);
                } else {
                    request.setAttribute("message", "添加失败");
                    request.getRequestDispatcher("AddOrAlterUser.jsp?").forward(request, response);
                }
            }
        }
        //修改用户
        else{
            String namebefore=request.getParameter("uname");
            if(name==null||name.equals("")||password==null||password.equals("")){
                request.setAttribute("message", "不能输入空字段！");
                request.getRequestDispatcher("AddOrAlterUser.jsp").forward(request, response);
            }
            else {
                if (new Admin().UpdateUser(namebefore, name, password)) {
                    request.setAttribute("message", "修改成功");
                    User user = new Search().SearchUserbyname(name);
                    request.setAttribute("user", user);
                    request.getRequestDispatcher("AddOrAlterUser.jsp").forward(request, response);
                } else {
                    request.setAttribute("message", "修改失败");
                    User user = new Search().SearchUserbyname(namebefore);
                    request.setAttribute("user", user);
                    request.getRequestDispatcher("AddOrAlterUser.jsp").forward(request, response);
                }
            }
        }
    }
}
