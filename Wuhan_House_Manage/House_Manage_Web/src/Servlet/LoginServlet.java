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

@WebServlet(name = "LoginServlet", value = "/LoginServlet")
public class LoginServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");
        //获得request传来的参数
        String username = request.getParameter("name");
        String password = request.getParameter("password");
        String choice=request.getParameter("R1");
        if(username.isEmpty()||password.isEmpty()){
            request.setAttribute("message","不能输入空字段！");
            request.getRequestDispatcher("Login.jsp").forward(request, response);
            return;
        }

        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()) {
                Statement statement=connection.createStatement();
                String sql;
                ResultSet resultSet;
                switch (choice) {
                    case "游客":
                        sql="select password from h_user where username='"+username+"';";
                        resultSet=statement.executeQuery(sql);
                        if(resultSet.next()){
                            String p=resultSet.getString(1).replace(" ","");
                            if(password.equals(p)){
                                request.getSession().setAttribute("name",username);
                                request.getSession().setAttribute("type","user");
                                response.sendRedirect("User.jsp");
                                return;
                            }
                        }
                        request.setAttribute("message","登录失败！账号或密码错误！");
                        request.getRequestDispatcher("Login.jsp").forward(request, response);
                        break;
                    case "企业用户":
                        sql="select password from h_company where username='"+username+"';";
                        resultSet=statement.executeQuery(sql);
                        if(resultSet.next()){
                            String p=resultSet.getString(1).replace(" ","");
                            if(password.equals(p)){
                                request.getSession().setAttribute("name",username);
                                request.getSession().setAttribute("type","company");
                                response.sendRedirect("Company.jsp");
                                return;
                            }
                        }
                        request.setAttribute("message","登录失败！账号或密码错误！");
                        request.getRequestDispatcher("Login.jsp").forward(request, response);
                        break;
                    case "管理员":
                        sql="select password from h_admin where adminname='"+username+"';";
                        resultSet=statement.executeQuery(sql);
                        if(resultSet.next()){
                            String p=resultSet.getString(1).replace(" ","");
                            if(password.equals(p)){
                                request.getSession().setAttribute("name",username);
                                request.getSession().setAttribute("type","admin");
                                response.sendRedirect("Admin.jsp");
                                return;
                            }
                        }
                        request.setAttribute("message","登录失败！账号或密码错误！");
                        request.getRequestDispatcher("Login.jsp").forward(request, response);
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
