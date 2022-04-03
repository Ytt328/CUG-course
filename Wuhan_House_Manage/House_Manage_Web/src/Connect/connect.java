package Connect;

import java.sql.Connection;
import java.sql.DriverManager;

/**
 * 连接到数据库
 */
public class connect {
    /**
     * 连接数据库
     * @return 连接
     */
    public Connection getconnet(){
        Connection connection;
        try {
            String url="jdbc:sqlserver://localhost:1433;DatabaseName=House;";
            Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
            connection = DriverManager.getConnection(url, "sa", "524670328yt");

        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        return connection;
    }
}