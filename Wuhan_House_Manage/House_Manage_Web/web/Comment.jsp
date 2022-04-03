<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/27
  Time: 18:40
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>评论</title>
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
<form action="CommentServlet?id=${id}" method="post">
    <table border="1" style="margin: auto;border-collapse: collapse;">
        <tr style="background-color: dodgerblue">
            <th colspan="2">评论${id}号房源</th>
        </tr>
        <tr>
            <th>输入评论</th>
            <th><input type="text" name="comment" size="30"></th>
        </tr>
        <tr>
            <th colspan="2">
                <input type="submit" value="提交">
                <input type="reset" value="取消" onclick="history.back(-1)">
            </th>
        </tr>
    </table>

</form>
</body>
</html>
