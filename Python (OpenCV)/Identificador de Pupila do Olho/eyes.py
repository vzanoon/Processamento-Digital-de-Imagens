cam = cv2.VideoCapture('Eyes.mp4')
cv2.namedWindow('IMG')
ret, frame = cam.read()
fourcc = cv2.VideoWriter_fourcc(*'XVID')
#print(fourcc)
out = cv2.VideoWriter('output.MP4', fourcc, 35.0, (500,300))
while ret:

    frame = cv2.resize(frame, (500, 300))
    frame_filter = cv2.medianBlur(frame, 15)
    frame_gray = cv2.cvtColor(frame_filter, cv2.COLOR_BGR2GRAY)
    frame_hsv = cv2.cvtColor(frame_filter, cv2.COLOR_BGR2HSV)

    # Pela cor da pele delimita-se o contorno do olho
    frame_bin = cv2.inRange(frame_hsv, (0, 45, 4), (15, 255, 255))
    frame_bin2 = cv2.inRange(frame_hsv, (174, 45, 4), (179, 255, 255))
    frame_bin = frame_bin + frame_bin2
    frame_bin = cv2.morphologyEx(frame_bin, cv2.MORPH_OPEN, np.ones((3, 3), np.uint8))
    frame_bin = cv2.morphologyEx(frame_bin, cv2.MORPH_DILATE, np.ones((3, 3), np.uint8))

    frame_bin = cv2.bitwise_not(frame_bin)
    contorns = cv2.findContours(frame_bin, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)[0]

    if not (contorns == []):
        cnt = max(contorns, key=cv2.contourArea)
        approx = cv2.convexHull(cnt)
        cv2.drawContours(frame, [approx], -1, (0, 255, 255), 2, cv2.LINE_AA)
        moments = cv2.moments(approx)
        area = moments['m00']
        if area > 0:
            #  Centro de Massa do Olho
            centerX = moments['m10'] / area
            centerY = moments['m01'] / area
            cv2.circle(frame, (int(centerX), int(centerY)), 20, (255, 0, 0), 1, cv2.LINE_AA)
            cv2.line(frame, (int(centerX), 0 + 30), (int(centerX), 300 - 30), (255, 0, 0), 1, cv2.LINE_AA)
            cv2.line(frame, (0 + 30, int(centerY)), (500 - 30, int(centerY)), (255, 0, 0), 1, cv2.LINE_AA)

            #  Identificacao da Pupila
            circles = cv2.HoughCircles(frame_gray, cv2.HOUGH_GRADIENT, dp=1, minDist=1000, param1=40, param2=30,
                                       minRadius=15, maxRadius=40)

            if circles is None:
                out.write(frame)
                cv2.imshow('IMG', frame)
                #cv2.imshow('HSV', frame_bin)
                if cv2.waitKey(1) == 27:
                    break

                ret, frame = cam.read()
                continue

            for circle in circles[0, :]:
                cv2.circle(frame, (int(circle[0]), int(circle[1])), int(circle[2]), (0, 255, 0), 2, cv2.LINE_AA)
                cv2.circle(frame, (int(circle[0]), int(circle[1])), 2, (0, 0, 255), 2, cv2.LINE_AA)
                cv2.line(frame, (int(circle[0]), int(circle[1])), (int(centerX), int(centerY)), (255, 0, 255), 2, cv2.LINE_AA)
                dist = ((circle[0] - centerX) ** 2 + (circle[1] - centerY) ** 2) ** (1 / 2)

                # Quadrante que delimita, conforme o angulo, as direcoes CENTER, RIGHT, LEFT, TOP e DOWN
                vetorAB = [1, 0]
                vetorCD = [circle[0] - centerX, circle[1] - centerY]
                angulo = math.acos(((vetorAB[0] * vetorCD[0]) + (vetorAB[1] * vetorCD[1])) / (
                        math.hypot(vetorAB[0], vetorAB[1]) * math.hypot(vetorCD[0], vetorCD[1]))) * 180 / math.pi

                if vetorCD[1] > 0:
                    angulo = 360 - angulo

                if dist < 20:
                    cv2.putText(frame, 'CENTER', (10, 30), cv2.FONT_HERSHEY_PLAIN, 1.5, (255, 255, 255))
                    cv2.putText(frame, 'Angle: '+str(round(angulo))+ ' graus', (10,50), cv2.FONT_HERSHEY_PLAIN, 1.0,
                                     (255,255,255))
                else:
                    if angulo > 45 and angulo <= 135:
                        cv2.putText(frame, 'TOP', (10, 30), cv2.FONT_HERSHEY_PLAIN, 1.5, (255, 255, 255))
                        cv2.putText(frame, 'Angle: ' + str(round(angulo))+' graus', (10, 50), cv2.FONT_HERSHEY_PLAIN, 1.0,
                                    (255, 255, 255))
                    if angulo > 135 and angulo <= 225:
                        cv2.putText(frame, 'RIGHT', (10, 30), cv2.FONT_HERSHEY_PLAIN, 1.5, (255, 255, 255))
                        cv2.putText(frame, 'Angle: ' + str(round(angulo))+' graus', (10, 50), cv2.FONT_HERSHEY_PLAIN, 1.0,
                                    (255, 255, 255))
                    if angulo > 225 and angulo <= 315:
                        cv2.putText(frame, 'DOWN', (10, 30), cv2.FONT_HERSHEY_PLAIN, 1.5, (255, 255, 255))
                        cv2.putText(frame, 'Angle: ' + str(round(angulo))+' graus', (10, 50), cv2.FONT_HERSHEY_PLAIN, 1.0,
                                    (255, 255, 255))
                    if angulo > 315 or angulo <= 45:
                        cv2.putText(frame, 'LEFT', (10, 30), cv2.FONT_HERSHEY_PLAIN, 1.5, (255, 255, 255))
                        cv2.putText(frame, 'Angle: ' + str(round(angulo))+' graus', (10, 50), cv2.FONT_HERSHEY_PLAIN, 1.0,
                                    (255, 255, 255))
    out.write(frame)
    cv2.imshow('IMG', frame)
    #cv2.imshow('HSV', frame_bin)
    if cv2.waitKey(1) == 27:
        break

    ret, frame = cam.read()

cam.release()
out.release()
cv2.destroyAllWindows()
