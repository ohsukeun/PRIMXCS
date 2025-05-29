package egovframework.com.primx.fun.sys.service;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class FileZoneVO {
    private int zoneId; // ZONE ID
    private String zoneNm; // ZONE명
    private String proto; // 프로토콜
    private String zoneData; // Zone 관련 데이터(JSON)

    // JSON 직렬화/역직렬화를 위한 toString() 오버라이드
    @Override
    public String toString() {
        return "FileZoneVO{" +
                "zoneId=" + zoneId +
                ", zoneNm='" + zoneNm + '\'' +
                ", proto='" + proto + '\'' +
                ", zoneData='" + zoneData + '\'' +
                '}';
    }
}
