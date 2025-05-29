package egovframework.com.primx.fun.file.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class MoveFileSearchDTO extends DataTableBaseVO  {
    
    private String dateSelect;
    private String startDt;
    private String endDt;
    private String status;
    private String condition;
    private String searchKeyword;
    
    public void convertUserTimeToUTC() {
        this.startDt = DateUtil.convertUserTimeToUTC(this.startDt);
        this.endDt = DateUtil.convertUserTimeToUTC(this.endDt);
    }
    
}
