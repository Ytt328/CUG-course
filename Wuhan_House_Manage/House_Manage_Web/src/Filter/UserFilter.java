package Filter;

import javax.servlet.*;
import javax.servlet.annotation.*;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

/**
 * 只有游客才能进行评论
 * 只有游客才能进入User.jsp
 */

@WebFilter(filterName = "UserFilter",urlPatterns = {"/User.jsp","/Comment.jsp"})
public class UserFilter implements Filter {
    public void init(FilterConfig config) throws ServletException {
    }

    public void destroy() {
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws ServletException, IOException {
        HttpServletRequest httpServletRequest= (HttpServletRequest) request;
        String name= (String) httpServletRequest.getSession().getAttribute("name");
        String type=(String) httpServletRequest.getSession().getAttribute("type");
        if(name==null||name.equals("")||type==null||type.equals("")||!type.equals("user")){
            request.setAttribute("message","无权限访问,请登录,如果已登录,请切换用户");
            request.getRequestDispatcher("Login.jsp").forward(request,response);
            chain.doFilter(request, response);
        }
        else {
            chain.doFilter(request, response);
        }
    }
}
