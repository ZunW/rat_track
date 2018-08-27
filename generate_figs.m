%read in data
filename='clip9_warp_human_centroid.txt';
delimiter = ' ';
humanData = importdata(filename, delimiter);
filename = 'clip9_warp_algo.txt';
algoData = importdata(filename, delimiter);
humanX = humanData(:, 1);
humanY = humanData(:, 2);
algoX = algoData(:, 1);
algoY = algoData(:, 2);

%linear interpolate the human data
humanOrigInd = (0:50:5851)';
fullIndex = (0:1:5851)';
humanInterpX = interp1q(humanOrigInd, humanX, fullIndex);
humanInterpX(5852, 1) = humanInterpX(5851, 1);
humanInterpY = interp1q(humanOrigInd, humanY, fullIndex);
humanInterpY(5852, 1) = humanInterpY(5851, 1);

%generate x fig
xFig = figure();
plot(algoX, 'LineWidth', 1);
title('Algorithm Generated vs. Human Marked (Interpolated) X coordinates');
xlabel('Time (number of frame)');
ylabel('X coordinate');
hold on;
plot(humanInterpX, 'LineWidth', 1);
ylim([0, 550]);
legend('Algorithm Generated', 'Human Marked (Interpolated)');

%generate y fig
yFig = figure();
plot(algoY, 'LineWidth', 1);
title('Algorithm Generated vs. Human Marked (Interpolated) Y coordinates');
xlabel('Time (number of frame)');
ylabel('Y coordinate');
hold on;
plot(humanInterpY, 'LineWidth', 1);
ylim([0, 550]);
ylim([0, 650]);
legend('Algorithm Generated', 'Human Marked (Interpolated)');

