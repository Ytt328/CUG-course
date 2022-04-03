<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ page isELIgnored="false" %>
<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/28
  Time: 14:09
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>管理员后台管理</title>
</head>

<%
    String message = (String) request.getAttribute("message");
    request.getSession().setAttribute("message",null);
    if((message!=null)&&!message.equals("")){
%>
<script type="text/javascript">
    alert("<%=message%>");
</script>
<%
    }
%>
<body>
<div style="width: -moz-max-content">
    <div id="menu1" style="width: 50%;height: auto;float:left;border: #cccccc">
        <div style="width: 100%;height: auto;float: top;text-align: center">
            <label style="font-size: larger;font-style: revert">游客管理</label>
            <a href="AddOrAlterUser.jsp" style="float: left;color: green">增加游客</a>
        </div>
        <table border="1">
            <tr>
                <th colspan="3" style="background-color: #2175BC">游客信息</th>
            </tr>
            <tr style="background-color: #DBEBD3">
                <td width="200">账号</td>
                <td width="200">密码</td>
                <td width="200">操作</td>
            </tr>
            <c:forEach items="${UL}" var="user">
                <tr>
                    <td>${user.name}</td>
                    <td>${user.password}</td>
                    <td>
                        <a href="BackgroundServlet?type=alterU&name=${user.name}" style="color: #2175BC">修改</a>
                        <a href="BackgroundServlet?type=deleteU&name=${user.name}" style="color: red">删除</a>
                    </td>
                </tr>
            </c:forEach>
        </table>
    </div>
    <div id="menu2" style="width: 50%;height: auto;float: left;border: #cccccc">
        <div style="width: 100%;height: auto;float: top;text-align: center">
            <label style="font-size: larger;font-style: revert">企业用户管理</label>
            <a href="AddOrAlterCom.jsp" style="float: left;color: green">增加企业用户</a>
        </div>
        <table border="1">
            <tr>
                <th colspan="3" style="background-color: #2175BC">企业用户信息</th>
            </tr>
            <tr style="background-color: #DBEBD3">
                <td width="200">账号</td>
                <td width="200">密码</td>
                <td width="200">操作</td>
            </tr>
            <c:forEach items="${CL}" var="company">
                <tr>
                    <td>${company.name}</td>
                    <td>${company.password}</td>
                    <td>
                        <a href="BackgroundServlet?type=alterC&name=${company.name}" style="color: #2175BC">修改</a>
                        <a href="BackgroundServlet?type=deleteC&name=${company.name}" style="color: red">删除</a>
                    </td>
                </tr>
            </c:forEach>
        </table>
    </div>
</div>
</body>
</html>
