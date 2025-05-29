package egovframework.com.primx.fun.cmm.service;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class AlarmVO {
    private Long seqNo;
    private String userId;
    private String userNm;
    private String path;
    private String type;
    private String typeDesc;
    private String userRole;
    private String insertDate;
    private int isRead;
    private String targetId;
}
