package Filter;

import javax.servlet.*;
import javax.servlet.annotation.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

/**
 * 只有企业用户才能对小区和房源进行增删改
 * 企业用户才能进入Company.jsp
 * 企业用户才能进入修改/添加小区或者房源的界面
 */

@WebFilter(filterName = "CompanyFilter",urlPatterns = {"/Company.jsp","/AddOrAlterHouse.jsp","/AddOrAlterNeigh.jsp"})
public class CompanyFilter implements Filter {
    public void init(FilterConfig config) throws ServletException {
    }

    public void destroy() {
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws ServletException, IOException {
        HttpServletRequest httpServletRequest= (HttpServletRequest) request;
        String name= (String) httpServletRequest.getSession().getAttribute("name");
        String type=(String) httpServletRequest.getSession().getAttribute("type");
        if(name==null||name.equals("")||type==null||type.equals("")||!type.equals("company")){
            request.setAttribute("message","无权限访问,请登录,如果已登录,请切换用户");
            request.getRequestDispatcher("Login.jsp").forward(request,response);
            chain.doFilter(request, response);
        }
        else {
            chain.doFilter(request, response);
        }
    }
}
