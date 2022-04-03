package Filter;

import javax.servlet.*;
import javax.servlet.annotation.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

/**
 * 只有管理员才能进入Admin.jsp
 * 只有管理员才能进入后台Background.jsp
 * 只有管理员才能对游客和企业用户进行增删改
 */

@WebFilter(filterName = "AdminFilter",urlPatterns = {"/Admin.jsp","/Background.jsp","/AddOrAlterUser.jsp","/AddOrAlterCom.jsp"})
public class AdminFilter implements Filter {
    public void init(FilterConfig config) throws ServletException {
    }

    public void destroy() {
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws ServletException, IOException {
        HttpServletRequest httpServletRequest= (HttpServletRequest) request;
        String name= (String) httpServletRequest.getSession().getAttribute("name");
        String type=(String) httpServletRequest.getSession().getAttribute("type");
        if(name==null||name.equals("")||type==null||type.equals("")||!type.equals("admin")){
            request.setAttribute("message","无权限访问,请登录,如果已登录,请切换用户");
            request.getRequestDispatcher("Login.jsp").forward(request,response);
            chain.doFilter(request, response);
        }
        else {
            chain.doFilter(request, response);
        }
    }
}
