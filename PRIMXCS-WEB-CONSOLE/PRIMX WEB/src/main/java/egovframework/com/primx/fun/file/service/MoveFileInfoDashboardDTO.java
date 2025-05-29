package egovframework.com.primx.fun.file.service;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;


@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class MoveFileInfoDashboardDTO {
    
    private String dayLabel = "";
    private String weekLabel = "";
    private String monthLabel = "";
    private int dayCreatedCnt = 0;
    private int dayUsedCnt = 0;
    private int dayExpiredCnt = 0;
    private int weekCreatedCnt = 0;
    private int weekUsedCnt = 0;
    private int weekExpiredCnt = 0;
    private int monthCreatedCnt = 0;
    private int monthUsedCnt = 0;
    private int monthExpiredCnt = 0;
    
    public static MoveFileInfoDashboardDTO convertMoveFileStatus(List<MoveFileInfoDTO> moveFileInfoDTOList, int maxCnt) {
        MoveFileInfoDashboardDTO dto = MoveFileInfoDashboardDTO.builder().build();
        
        String end = LocalDate.now().toString();
        dto.setDayLabel(LocalDate.now().toString());
        dto.setWeekLabel(LocalDate.now().minusDays(7).toString());
        dto.setMonthLabel(LocalDate.now().minusDays(30).toString());
        dto.setDayCnt(moveFileInfoDTOList, LocalDate.now().toString() + "T00:00:00", end + "T23:59:59", maxCnt);
        dto.setWeekCnt(moveFileInfoDTOList, LocalDate.now().minusDays(7).toString() + "T00:00:00", end + "T23:59:59", maxCnt);
        dto.setMonthCnt(moveFileInfoDTOList, LocalDate.now().minusDays(30).toString() + "T00:00:00", end + "T23:59:59", maxCnt);
        
        return dto;
    }

    private void setDayCnt(List<MoveFileInfoDTO> moveFileInfoDTOList, String start, String end, int maxCnt) {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime startTime = LocalDateTime.parse(start.replace("T", " "), formatter);
        LocalDateTime endTime = LocalDateTime.parse(end.replace("T", " "), formatter);
        
        moveFileInfoDTOList.stream().forEach(dto -> {

            if (dto.getInsertDate() == null) return;

            LocalDateTime insertTime = LocalDateTime.parse(dto.getInsertDate(), formatter);

            if (!insertTime.isBefore(startTime) && !insertTime.isAfter(endTime)) {
                dayCreatedCnt++;
                if(Integer.parseInt(dto.getDecryptCnt()) == 0) {
                    dayExpiredCnt++;
                    return;
                }
                else if(dto.getExpireDate() != null && dto.getExpireDate().compareTo(end) < 0) {
                    dayExpiredCnt++;
                    return;
                }
                else if(Integer.parseInt(dto.getDecryptCnt()) < maxCnt) {
                    dayUsedCnt++;
                }
            }
        });
    }
    
    private void setWeekCnt(List<MoveFileInfoDTO> moveFileInfoDTOList, String start, String end, int maxCnt) {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime startTime = LocalDateTime.parse(start.replace("T", " "), formatter);
        LocalDateTime endTime = LocalDateTime.parse(end.replace("T", " "), formatter);
        
        moveFileInfoDTOList.stream().forEach(dto -> {

            if (dto.getInsertDate() == null) return;

            LocalDateTime insertTime = LocalDateTime.parse(dto.getInsertDate(), formatter);

            if (!insertTime.isBefore(startTime) && !insertTime.isAfter(endTime)) {
                weekCreatedCnt++;
                if(Integer.parseInt(dto.getDecryptCnt()) == 0) {
                    weekExpiredCnt++;
                    return;
                }
                else if(dto.getExpireDate() != null && dto.getExpireDate().compareTo(end) < 0) {
                    weekExpiredCnt++;
                    return;
                }
                else if(Integer.parseInt(dto.getDecryptCnt()) < maxCnt) {
                    weekUsedCnt++;
                }
            }
        });
    }

    private void setMonthCnt(List<MoveFileInfoDTO> moveFileInfoDTOList, String start, String end, int maxCnt) {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime startTime = LocalDateTime.parse(start.replace("T", " "), formatter);
        LocalDateTime endTime = LocalDateTime.parse(end.replace("T", " "), formatter);
        
        moveFileInfoDTOList.stream().forEach(dto -> {

            if (dto.getInsertDate() == null) return;

            LocalDateTime insertTime = LocalDateTime.parse(dto.getInsertDate(), formatter);

            if (!insertTime.isBefore(startTime) && !insertTime.isAfter(endTime)) {
                monthCreatedCnt++;
                if(Integer.parseInt(dto.getDecryptCnt()) == 0) {
                    monthExpiredCnt++;
                    return;
                }
                else if(dto.getExpireDate() != null && dto.getExpireDate().compareTo(end) < 0) {
                    monthExpiredCnt++;
                    return;
                }
                else if(Integer.parseInt(dto.getDecryptCnt()) < maxCnt) {
                    monthUsedCnt++;
                }
            }
        });
    }
}
