package Servlet;

import Entity.Admin;
import Entity.Company;
import Entity.User;
import Function.Search;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "BackgroundServlet", value = "/BackgroundServlet")
public class BackgroundServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");
        //修改游客信息
        if(request.getParameter("type").equals("alterU")){
            String name=request.getParameter("name");
            User user=new Search().SearchUserbyname(name);
            request.setAttribute("user",user);
            request.getRequestDispatcher("AddOrAlterUser.jsp").forward(request,response);
        }
        //删除游客
        else if(request.getParameter("type").equals("deleteU")){
            String name=request.getParameter("name");
            if(new Admin().DeleteUser(name)){
                request.setAttribute("message","删除成功！");
                request.getRequestDispatcher("Background.jsp").forward(request,response);
            }
            else{
                request.setAttribute("message","删除失败！该用户可能已经被删除");
                request.getRequestDispatcher("Background.jsp").forward(request,response);
            }
        }
        //修改企业用户信息
        else if(request.getParameter("type").equals("alterC")){
            String name=request.getParameter("name");
            Company company=new Search().SearchCompanybyname(name);
            request.setAttribute("company",company);
            request.getRequestDispatcher("AddOrAlterCom.jsp").forward(request,response);
        }
        //删除企业用户
        else if(request.getParameter("type").equals("deleteC")){
            String name=request.getParameter("name");
            if(new Admin().DeleteCompany(name)){
                request.setAttribute("message","删除成功！");
                request.getRequestDispatcher("Background.jsp").forward(request,response);
            }
            else{
                request.setAttribute("message","删除失败！该企业用户可能已经被删除");
                request.getRequestDispatcher("Background.jsp").forward(request,response);
            }
        }
    }
}
