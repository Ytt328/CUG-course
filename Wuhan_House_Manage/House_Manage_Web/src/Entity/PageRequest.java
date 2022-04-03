package Entity;

import Entity.House;

import java.util.List;

/**
 * 分页查询工具类
 */
public class PageRequest {
    int totalcount;//总条数
    int totalpage;//总页数
    int currentpage;//当前页数
    int pre;//上一页
    int next;//下一页
    int pagenum=10;//每一页显示的条数
    List<House> rs;//查询结果

    public int getPre() {
        return pre;
    }

    public void setPre(int pre) {
        this.pre = pre;
    }

    public int getNext() {
        return next;
    }

    public void setNext(int next) {
        this.next = next;
    }

    public int getTotalcount() {
        return totalcount;
    }

    public void setTotalcount(int totalcount) {
        this.totalcount = totalcount;
    }

    public int getTotalpage() {
        return totalpage;
    }

    public void setTotalpage(int totalpage) {
        this.totalpage = totalpage;
    }

    public int getCurrentpage() {
        return currentpage;
    }

    public void setCurrentpage(int currentpage) {
        this.currentpage = currentpage;
    }

    public List<House> getRs() {
        return rs;
    }

    public void setRs(List<House> rs) {
        this.rs = rs;
    }
}
