package Servlet;

import Connect.connect;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

@WebServlet(name = "RegisterServlet", value = "/RegisterServlet")
public class RegisterServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");

        String username=request.getParameter("name");
        String password1=request.getParameter("password1");
        String password2=request.getParameter("password2");
        String choice=request.getParameter("R1");
        if(username.isEmpty()||password1.isEmpty()||password2.isEmpty()){
            request.getSession().setAttribute("message","不能输入空字段！");
            request.getRequestDispatcher("Register.jsp").forward(request, response);
        }
        if(username.length()>20||password1.length()>20||password2.length()>20){
            request.getSession().setAttribute("message","用户名和密码长度不能超过20！");
            request.getRequestDispatcher("Register.jsp").forward(request, response);
        }
        if(!password1.equals(password2)){
            request.getSession().setAttribute("message","两次输入的密码不一致！");
            request.getRequestDispatcher("Register.jsp").forward(request, response);
        }
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                String sql;
                ResultSet resultSet;
                switch (choice){
                    case "游客":
                        sql="select * from h_user where username='"+username+"';";
                        resultSet=statement.executeQuery(sql);
                        if(resultSet.next()){
                            request.setAttribute("message","该用户已存在，请尝试修改账号！");
                            request.getRequestDispatcher("Register.jsp").forward(request, response);
                        }
                        else{
                            sql="insert into h_user(username,password) values('"+username+"','"+password1+"');";
                            statement.execute(sql);
                            request.setAttribute("message","注册成功");
                            request.getRequestDispatcher("Register.jsp").forward(request, response);
                            return;
                        }
                        break;
                    case "企业用户":
                        sql="select * from h_company where username='"+username+"';";
                        resultSet=statement.executeQuery(sql);
                        if(resultSet.next()){
                            request.setAttribute("message","该用户已存在，请尝试修改账号！");
                            request.getRequestDispatcher("Register.jsp").forward(request, response);
                        }
                        else{
                            sql="insert into h_company values('"+username+"','"+password1+"');";
                            statement.execute(sql);
                            request.setAttribute("message","注册成功");
                            request.getRequestDispatcher("Register.jsp").forward(request, response);
                            return;
                        }
                        break;
                    case "管理员":
                        sql="select * from h_admin where adminname='"+username+"';";
                        resultSet=statement.executeQuery(sql);
                        if(resultSet.next()){
                            request.setAttribute("message","该用户已存在，请尝试修改账号！");
                            request.getRequestDispatcher("Register.jsp").forward(request, response);
                        }
                        else{
                            sql="insert into h_admin values('"+username+"','"+password1+"');";
                            statement.execute(sql);
                            request.setAttribute("message","注册成功");
                            request.getRequestDispatcher("Register.jsp").forward(request, response);
                            return;
                        }
                        break;
                    default:
                        break;
                }
                statement.close();
                connection.close();
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
    }
}
