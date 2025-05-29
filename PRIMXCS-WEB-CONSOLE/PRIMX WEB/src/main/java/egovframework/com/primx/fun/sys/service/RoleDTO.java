package egovframework.com.primx.fun.sys.service;

public class RoleDTO {
    private String role;
    private String roleName;

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public String getRoleName() {
        return roleName;
    }

    public void setRoleName(String roleName) {
        this.roleName = roleName;
    }

    public String toString() {
        return "RoleDTO [role=" + role + ", roleName=" + roleName + "]";
    }
}
