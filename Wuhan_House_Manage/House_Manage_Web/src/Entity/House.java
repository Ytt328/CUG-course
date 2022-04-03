package Entity;

/**
 * 房源
 */
public class House {
    private int ID;//房源ID
    private String housetype;//房型
    private String price;//总价
    private String avgprice;//均价
    private String size;//建筑面积
    private String innersize;//套内面积
    private String towards;//朝向
    private String nname;//小区名
    private String tfloor;//楼层
    private String hfloor;//总楼层
    private String buildyear;//建筑年份
    private String buildtype;//建筑类型
    private String comment;//评论
    private String area;//行政区

    public int getID() {
        return ID;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public String getHousetype() {
        return housetype;
    }

    public void setHousetype(String housetype) {
        this.housetype = housetype;
    }

    public String getPrice() {
        return price;
    }

    public void setPrice(String price) {
        this.price = price;
    }

    public String getAvgprice() {
        return avgprice;
    }

    public void setAvgprice(String avgprice) {
        this.avgprice = avgprice;
    }

    public String getSize() {
        return size;
    }

    public void setSize(String size) {
        this.size = size;
    }

    public String getInnersize() {
        return innersize;
    }

    public void setInnersize(String innersize) {
        this.innersize = innersize;
    }

    public String getTowards() {
        return towards;
    }

    public void setTowards(String towards) {
        this.towards = towards;
    }

    public String getNname() {
        return nname;
    }

    public void setNname(String nname) {
        this.nname = nname;
    }

    public String getTfloor() {
        return tfloor;
    }

    public void setTfloor(String tfloor) {
        this.tfloor = tfloor;
    }

    public String getHfloor() {
        return hfloor;
    }

    public void setHfloor(String hfloor) {
        this.hfloor = hfloor;
    }

    public String getBuildyear() {
        return buildyear;
    }

    public void setBuildyear(String buildyear) {
        this.buildyear = buildyear;
    }

    public String getBuildtype() {
        return buildtype;
    }

    public void setBuildtype(String buildtype) {
        this.buildtype = buildtype;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public String getArea() {
        return area;
    }

    public void setArea(String area) {
        this.area = area;
    }


}
