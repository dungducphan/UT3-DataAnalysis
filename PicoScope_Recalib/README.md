
Data taken on 2025-01-14 on 2 oscilloscopes:
    - LeCroy, CH2.
    - PicoScope, CHB.

Shot log: https://utexas.app.box.com/file/1749792525115
    - 50 shots for LeCroy: 75-123 (missing 1 shot)
    - 50 shots for PicoScope: 134-183

Hard coded in the PicoScope file reader that the first 500 data points (contains only pedestal) are removed.

Integrate Domain:
    - LeCroy: 1751-end
    - PicoScope: 1202-end



# Update on 2025-02-20
- We took some data for Artranis on 2025-02-19, ICT and LANEX data are now included in this repository.
- Modifying the code to correlate the data from ICT and LANEX and calculate the TID on DUT.
- Thanh's code live independently "somewhere else", I will just generate a dose table for him to continue the analysis.