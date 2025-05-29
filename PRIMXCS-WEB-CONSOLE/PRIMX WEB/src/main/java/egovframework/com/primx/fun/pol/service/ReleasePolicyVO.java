package egovframework.com.primx.fun.pol.service;

import java.util.Date;

import lombok.Data;

@Data
public class ReleasePolicyVO  {
    private int regId; // 자동 생성
    private String deptId; // 부서 정보
    private String deptName; // 부서 이름
    private String deptPath; // 부서 경로
    private Date beginDate; // 시작일
    private Date endDate; // 종료일
    private int computerCnt; // 컴퓨터 수
    private int latestUpdateCnt; // 최신 업데이트 개수
    private String releaseYn; // 배포 여부 Y: 배포, N: 중단
}
