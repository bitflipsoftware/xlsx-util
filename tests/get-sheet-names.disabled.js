const xlsx = require('../index')
const path = require('path')

describe('getSheetNames', () => {
    it('should return sheet names', async (done) => {
        const filepath = path.join(__dirname, 'sheet.names.xlsx')
        const result = await xlsx.getSheetNames(filepath);
        
        expect(result).toBeTruthy();
        expect(result.length).toBe(3);
        expect(result[0]).toBe('blerp1');
        expect(result[1]).toBe('@#$(& ^WF');
        expect(result[2]).toBe('Sheet3');
        done();
    })
})
