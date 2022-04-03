package Servlet;

import Entity.User;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "CommentServlet", value = "/CommentServlet")
public class CommentServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");

        int id= Integer.parseInt(request.getParameter("id"));
        String name= (String) request.getSession().getAttribute("name");
        String detail=request.getParameter("comment");
        if(detail==null||detail.equals("")){
            request.setAttribute("message","输入不能为空");
            request.setAttribute("id",id);
            request.getRequestDispatcher("Comment.jsp?id="+id).forward(request,response);
        }
        User user=new User();
        user.setName(name);
        user.Comment(id,detail);
        request.setAttribute("id",id);
        request.setAttribute("message","评论成功！");
        request.getRequestDispatcher("Comment.jsp?id="+id).forward(request,response);
    }
}
