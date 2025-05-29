package egovframework.com.primx.fun.cmm.util;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import javax.servlet.http.HttpServletRequest;

import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

/**
 * 날짜 관련 유틸리티 클래스
 * @author 김영우
 */
public class DateUtil {
    
    /**
     * 사용자의 시간대의 날짜 문자열을 UTC로 변환
     * @param dateStr 변환할 날짜 문자열 (yyyy-MM-dd 형식)
     * @return UTC로 변환된 날짜 문자열
     */
    public static String convertUserTimeToUTC(String dateStr) {
        if (dateStr == null || dateStr.isEmpty()) {
            return null;
        }
        
        try {
            // 사용자의 시간대 정보를 가져옴
            ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
            if (attributes != null) {
                HttpServletRequest request = attributes.getRequest();
                String userTimeZone = request.getHeader("X-User-Timezone");
                
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                
                // 사용자의 시간대가 있으면 해당 시간대 사용, 없으면 기본값으로 KST 사용
                if (userTimeZone != null && !userTimeZone.isEmpty()) {
                    sdf.setTimeZone(TimeZone.getTimeZone(userTimeZone));
                } else {
                    sdf.setTimeZone(TimeZone.getTimeZone("Asia/Seoul"));
                }
                
                Date date = sdf.parse(dateStr);
                sdf.setTimeZone(TimeZone.getTimeZone("UTC"));
                return sdf.format(date);
            }
            
            // RequestContext가 없는 경우 기본 KST로 처리
            return convertKSTtoUTC(dateStr);
        } catch (Exception e) {
            return dateStr;
        }
    }
    
    /**
     * UTC 날짜를 사용자의 시간대에 맞게 변환
     * @param utcDate UTC 날짜 문자열
     * @return 사용자의 시간대에 맞게 변환된 날짜 문자열
     */
    public static String convertUTCtoUserTime(String utcDate) {
        if (utcDate == null || utcDate.isEmpty()) {
            return null;
        }
        
        try {
            // 사용자의 시간대 정보를 가져옴
            ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
            if (attributes != null) {
                HttpServletRequest request = attributes.getRequest();
                String userTimeZone = request.getHeader("X-User-Timezone");
                
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                sdf.setTimeZone(TimeZone.getTimeZone("UTC"));
                Date date = sdf.parse(utcDate);
                
                // 사용자의 시간대가 있으면 해당 시간대 사용, 없으면 기본값으로 KST 사용
                if (userTimeZone != null && !userTimeZone.isEmpty()) {
                    sdf.setTimeZone(TimeZone.getTimeZone(userTimeZone));
                } else {
                    sdf.setTimeZone(TimeZone.getTimeZone("Asia/Seoul"));
                }
                
                return sdf.format(date);
            }
            
            // RequestContext가 없는 경우 기본 KST로 처리
            return convertUTCtoKST(utcDate);
        } catch (Exception e) {
            return utcDate;
        }
    }
    
    /**
     * 한국 시간대의 날짜 문자열을 UTC로 변환
     * @param dateStr 변환할 날짜 문자열 (yyyy-MM-dd 형식)
     * @return UTC로 변환된 날짜 문자열
     */
    public static String convertKSTtoUTC(String dateStr) {
        if (dateStr == null || dateStr.isEmpty()) {
            return null;
        }
        
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
            sdf.setTimeZone(TimeZone.getTimeZone("Asia/Seoul"));
            Date date = sdf.parse(dateStr);
            
            sdf.setTimeZone(TimeZone.getTimeZone("UTC"));
            return sdf.format(date);
        } catch (Exception e) {
            return dateStr;
        }
    }

    /**
     * UTC 날짜를 한국 시간대로 변환
     * @param utcDate UTC 날짜 문자열
     * @return 한국 시간대로 변환된 날짜 문자열
     */
    public static String convertUTCtoKST(String utcDate) {
        if (utcDate == null || utcDate.isEmpty()) {
            return null;
        }
        
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            sdf.setTimeZone(TimeZone.getTimeZone("UTC"));
            Date date = sdf.parse(utcDate);
            
            sdf.setTimeZone(TimeZone.getTimeZone("Asia/Seoul"));
            return sdf.format(date);
        } catch (Exception e) {
            return utcDate;
        }
    }
} 