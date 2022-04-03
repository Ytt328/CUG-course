<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/21
  Time: 8:50
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>注册</title>
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

<div style="text-align: center" margin:0 auto>
<body>
<form action="RegisterServlet" method="post">
    <h4><label>请输入用户名</label></h4>
    <h4><label><input type="text" name="name" size="20"></label></h4>
    <h4><label>请输入密码</label></h4>
    <h4><label><input type="password" name="password1" size="20"></label></h4>
    <h4><label>请确认密码</label></h4>
    <h4><label><input type="password" name="password2" size="20"></label></h4>
    <label><input type="radio" name="R1" value="游客" checked>游客</label>
    <label><input type="radio" name="R1" value="企业用户">企业用户</label>
    <label><input type="radio" name="R1" value="管理员">管理员</label>
    <p><input type="submit" value="注册"></p>
</form>
<form>
    <h5><a href="Login.jsp">返回登录界面</a> </h5>
</form>
</body>
</div>
</html>
