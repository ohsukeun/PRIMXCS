package egovframework.com.primx.fun.cmm.service;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;


@SuppressWarnings("serial")
public class DataTableBaseVO implements Serializable {

    /** DataTable draw (클라이언트 요청 ID) */
    private int draw;

    /** DataTable 시작 인덱스 */
    private int start;

    /** DataTable 데이터 길이 */
    private int length;

    /** 현재 페이지 */
    private int pageIndex = 1;

    /** 페이지당 레코드 수 */
    private int recordCountPerPage = 10;

    /** 검색 조건 (동적 필터) */
    private Map<String, String> searchFilters = new HashMap<>();

    public int getDraw() {
        return draw;
    }

    public void setDraw(int draw) {
        this.draw = draw;
    }

    public int getStart() {
        return start;
    }

    public void setStart(int start) {
        this.start = start;
        // 페이지 인덱스 계산
        this.pageIndex = (start / this.recordCountPerPage) + 1;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
        // 한 페이지에 표시할 레코드 수 설정
        this.recordCountPerPage = length;
    }

    public int getPageIndex() {
        return pageIndex;
    }

    public void setPageIndex(int pageIndex) {
        this.pageIndex = pageIndex;
    }

    public int getRecordCountPerPage() {
        return recordCountPerPage;
    }

    public void setRecordCountPerPage(int recordCountPerPage) {
        this.recordCountPerPage = recordCountPerPage;
    }

    public Map<String, String> getSearchFilters() {
        return searchFilters;
    }

    public void setSearchFilters(Map<String, String> searchFilters) {
        this.searchFilters = searchFilters;
    }

    /**
     * 검색 필터 추가
     *
     * @param key   검색 조건 키
     * @param value 검색 조건 값
     */
    public void addSearchFilter(String key, String value) {
        this.searchFilters.put(key, value);
    }
}