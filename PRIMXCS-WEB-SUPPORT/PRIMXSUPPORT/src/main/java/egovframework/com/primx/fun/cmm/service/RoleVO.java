package egovframework.com.primx.fun.cmm.service;

public class RoleVO {
    private String authorCode; // 권한코드
    private String authorName; // 권한명
    private String authorDescription; // 권한설명
    private String authorCreateDate; // 권한생성일

    // Getters and Setters
    public String getAuthorCode() {
        return authorCode;
    }

    public void setAuthorCode(String authorCode) {
        this.authorCode = authorCode;
    }

    public String getAuthorName() {
        return authorName;
    }

    public void setAuthorName(String authorName) {
        this.authorName = authorName;
    }

    public String getAuthorDescription() {
        return authorDescription;
    }

    public void setAuthorDescription(String authorDescription) {
        this.authorDescription = authorDescription;
    }

    public String getAuthorCreateDate() {
        return authorCreateDate;
    }

    public void setAuthorCreateDate(String authorCreateDate) {
        this.authorCreateDate = authorCreateDate;
    }

    @Override
    public String toString() {
        return "RoleVO [authorCode=" + authorCode + ", authorName=" + authorName + ", authorDescription="
                + authorDescription + ", authorCreateDate=" + authorCreateDate + "]";
    }
} 