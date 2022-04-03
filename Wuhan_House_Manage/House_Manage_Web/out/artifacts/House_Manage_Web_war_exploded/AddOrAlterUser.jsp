<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/28
  Time: 15:30
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>${user.name==null?"添加":"修改"}游客</title>
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
<form action="AddOrAlterUserServlet?uname=${user.name}" method="post">
<div style="text-align: center">
    <table border="1" style="margin: auto;border-collapse: collapse;">
        <tr style="background-color: dodgerblue">
            <th colspan="2">游客信息</th>
        </tr>
        <tr>
            <td>账号<label style="color: red">*</label> </td>
            <td><input type="text" size="20" name="name" value="${user.name}"></td>
        </tr>
        <tr>
            <td>密码<label style="color: red">*</label></td>
            <td><input type="text" size="20" name="password" value="${user.password}"></td>
        </tr>
        <tr>
            <th colspan="2">
                <button type="submit">保存</button>
                <button type="reset" onclick="history.back(-1)">取消</button>
            </th>
        </tr>
    </table>
</div>
</form>
</body>
</html>
